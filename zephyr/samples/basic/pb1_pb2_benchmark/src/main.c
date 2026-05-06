#include <stdint.h>
#include <stm32f4xx.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/time_units.h>

#define LOOPBACK_OUT_NODE DT_ALIAS(loopback_out)
#define LOOPBACK_IN_NODE  DT_ALIAS(loopback_in)

#if !DT_NODE_HAS_STATUS(LOOPBACK_OUT_NODE, okay)
#error "Unsupported board: loopback_out devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(LOOPBACK_IN_NODE, okay)
#error "Unsupported board: loopback_in devicetree alias is not defined"
#endif

#define GPIO_TOGGLE_COUNT        20U
#define GPIO_SETTLE_MS           1U
#define GPIO_HOLD_MS             20U
#define IRQ_TEST_PERIOD_US       1000U
#define IRQ_TEST_ITERATIONS      100U
#define SCHED_TEST_ITERATIONS    100U
#define SCHED_SLEEP_US           1000U
#define CTX_SWITCH_ITERATIONS    100U
#define CTX_STACK_SIZE           512U

static const struct gpio_dt_spec loopback_out = GPIO_DT_SPEC_GET(LOOPBACK_OUT_NODE, gpios);
static const struct gpio_dt_spec loopback_in = GPIO_DT_SPEC_GET(LOOPBACK_IN_NODE, gpios);

static struct k_sem irq_sem;
static struct k_sem sched_sem;
static struct k_timer sched_timer;
static struct k_sem ctx_ping_sem;
static struct k_sem ctx_pong_sem;

K_THREAD_STACK_DEFINE(ctx_ping_stack, CTX_STACK_SIZE);
K_THREAD_STACK_DEFINE(ctx_pong_stack, CTX_STACK_SIZE);
static struct k_thread ctx_ping_thread;
static struct k_thread ctx_pong_thread;

static volatile uint32_t boot_mark_cycle;
static volatile uint32_t main_entry_cycle;
static volatile uint32_t irq_expected_cycle;
static volatile uint32_t irq_measured_cycle;
static volatile uint32_t irq_sample_ready;
static volatile uint32_t sched_expected_cycle;
static volatile uint32_t sched_measured_cycle;
static volatile uint32_t ctx_switch_start_cycle;
static volatile uint32_t ctx_switch_end_cycle;
static volatile uint32_t ctx_switch_sum_cycles;
static volatile uint32_t ctx_switch_count;

static int boot_time_mark(void)
{
	boot_mark_cycle = k_cycle_get_32();
	return 0;
}

SYS_INIT(boot_time_mark, PRE_KERNEL_1, 0);

static uint32_t cycles_to_ns_u32(uint32_t cycles)
{
	uint64_t ns = k_cyc_to_ns_floor64(cycles);

	if (ns > UINT32_MAX) {
		return UINT32_MAX;
	}

	return (uint32_t)ns;
}

static int sample_and_check(int level, const char *tag)
{
	int ret;

	ret = gpio_pin_set_dt(&loopback_out, level);
	if (ret < 0) {
		printk("%s drive failed: %d\n", tag, ret);
		return ret;
	}

	k_msleep(GPIO_SETTLE_MS);

	ret = gpio_pin_get_dt(&loopback_in);
	if (ret < 0) {
		printk("%s sample failed: %d\n", tag, ret);
		return ret;
	}

	printk("%s drive=%d sample=%d %s\n",
	       tag, level, ret, (ret == level) ? "PASS" : "FAIL");

	return (ret == level) ? 0 : -EIO;
}

static void tim2_direct_isr(const void *arg)
{
	ARG_UNUSED(arg);

	if ((TIM2->SR & TIM_SR_UIF) != 0U) {
		TIM2->SR &= ~TIM_SR_UIF;
		irq_measured_cycle = k_cycle_get_32();
		irq_sample_ready = 1U;
		k_sem_give(&irq_sem);
	}
}

static void tim2_setup(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	__DSB();

	TIM2->CR1 = 0U;
	TIM2->PSC = 84U - 1U;
	TIM2->ARR = IRQ_TEST_PERIOD_US - 1U;
	TIM2->CNT = 0U;
	TIM2->EGR = TIM_EGR_UG;
	TIM2->SR = 0U;
	TIM2->DIER = TIM_DIER_UIE;

	IRQ_CONNECT(TIM2_IRQn, 0, tim2_direct_isr, NULL, 0);
	irq_enable(TIM2_IRQn);
}

static void sched_timer_expiry(struct k_timer *timer)
{
	ARG_UNUSED(timer);
	sched_measured_cycle = k_cycle_get_32();
	k_sem_give(&sched_sem);
}

static void ctx_ping_entry(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	while (1) {
		k_sem_take(&ctx_ping_sem, K_FOREVER);
		ctx_switch_end_cycle = k_cycle_get_32();
		ctx_switch_sum_cycles += (ctx_switch_end_cycle - ctx_switch_start_cycle);
		ctx_switch_count++;
		k_sem_give(&ctx_pong_sem);
	}
}

static void ctx_pong_entry(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	while (1) {
		k_sem_take(&ctx_pong_sem, K_FOREVER);
		ctx_switch_start_cycle = k_cycle_get_32();
		k_sem_give(&ctx_ping_sem);
	}
}

static void print_startup_time(void)
{
	uint32_t main_delta = main_entry_cycle - boot_mark_cycle;
	uint32_t now_delta = k_cycle_get_32() - boot_mark_cycle;

	printk("boot_to_main=%u cyc (%u ns)\n",
	       main_delta, cycles_to_ns_u32(main_delta));
	printk("boot_to_measure=%u cyc (%u ns)\n",
	       now_delta, cycles_to_ns_u32(now_delta));
}

static void run_irq_latency_test(void)
{
	uint32_t min_cycles = UINT32_MAX;
	uint32_t max_cycles = 0U;
	uint64_t sum_cycles = 0U;

	printk("irq_latency_direct iterations=%u period_us=%u\n",
	       IRQ_TEST_ITERATIONS, IRQ_TEST_PERIOD_US);

	for (uint32_t i = 1; i <= IRQ_TEST_ITERATIONS; i++) {
		uint32_t delta_cycles;

		irq_sample_ready = 0U;
		TIM2->CNT = 0U;
		TIM2->SR = 0U;
		irq_expected_cycle = k_cycle_get_32() + k_us_to_cyc_ceil32(IRQ_TEST_PERIOD_US);
		TIM2->CR1 |= TIM_CR1_CEN;

		k_sem_take(&irq_sem, K_FOREVER);
		TIM2->CR1 &= ~TIM_CR1_CEN;

		if (!irq_sample_ready) {
			printk("irq sample not ready\n");
			return;
		}

		delta_cycles = irq_measured_cycle - irq_expected_cycle;
		if (delta_cycles < min_cycles) {
			min_cycles = delta_cycles;
		}
		if (delta_cycles > max_cycles) {
			max_cycles = delta_cycles;
		}
		sum_cycles += delta_cycles;
	}

	printk("irq_latency_direct min=%u cyc (%u ns) max=%u cyc (%u ns) avg=%u cyc (%u ns)\n",
	       min_cycles,
	       cycles_to_ns_u32(min_cycles),
	       max_cycles,
	       cycles_to_ns_u32(max_cycles),
	       (uint32_t)(sum_cycles / IRQ_TEST_ITERATIONS),
	       cycles_to_ns_u32((uint32_t)(sum_cycles / IRQ_TEST_ITERATIONS)));
}

static void run_sched_latency_test(void)
{
	uint32_t min_cycles = UINT32_MAX;
	uint32_t max_cycles = 0U;
	uint64_t sum_cycles = 0U;

	printk("sched_latency iterations=%u sleep_us=%u\n",
	       SCHED_TEST_ITERATIONS, SCHED_SLEEP_US);

	for (uint32_t i = 1; i <= SCHED_TEST_ITERATIONS; i++) {
		uint32_t delta_cycles;

		sched_expected_cycle = k_cycle_get_32() + k_us_to_cyc_ceil32(SCHED_SLEEP_US);
		k_timer_start(&sched_timer, K_USEC(SCHED_SLEEP_US), K_NO_WAIT);
		k_sem_take(&sched_sem, K_FOREVER);
		delta_cycles = sched_measured_cycle - sched_expected_cycle;

		if (delta_cycles < min_cycles) {
			min_cycles = delta_cycles;
		}
		if (delta_cycles > max_cycles) {
			max_cycles = delta_cycles;
		}
		sum_cycles += delta_cycles;
	}

	printk("sched_latency min=%u cyc (%u ns) max=%u cyc (%u ns) avg=%u cyc (%u ns)\n",
	       min_cycles,
	       cycles_to_ns_u32(min_cycles),
	       max_cycles,
	       cycles_to_ns_u32(max_cycles),
	       (uint32_t)(sum_cycles / SCHED_TEST_ITERATIONS),
	       cycles_to_ns_u32((uint32_t)(sum_cycles / SCHED_TEST_ITERATIONS)));
}

static void run_ctx_switch_test(void)
{
	ctx_switch_sum_cycles = 0U;
	ctx_switch_count = 0U;

	k_sem_init(&ctx_ping_sem, 0, 1);
	k_sem_init(&ctx_pong_sem, 0, 1);

	k_thread_create(&ctx_ping_thread, ctx_ping_stack, CTX_STACK_SIZE,
			ctx_ping_entry, NULL, NULL, NULL,
			K_PRIO_PREEMPT(4), 0, K_NO_WAIT);
	k_thread_create(&ctx_pong_thread, ctx_pong_stack, CTX_STACK_SIZE,
			ctx_pong_entry, NULL, NULL, NULL,
			K_PRIO_PREEMPT(4), 0, K_NO_WAIT);

	printk("context_switch iterations=%u\n", CTX_SWITCH_ITERATIONS);

	ctx_switch_start_cycle = k_cycle_get_32();
	k_sem_give(&ctx_ping_sem);

	while (ctx_switch_count < CTX_SWITCH_ITERATIONS) {
		k_msleep(1);
	}

	printk("context_switch avg=%u cyc (%u ns)\n",
	       ctx_switch_sum_cycles / ctx_switch_count,
	       cycles_to_ns_u32(ctx_switch_sum_cycles / ctx_switch_count));

	k_thread_abort(&ctx_ping_thread);
	k_thread_abort(&ctx_pong_thread);
}

int main(void)
{
	int ret;
	int failures = 0;
	int pass_count = 0;

	main_entry_cycle = k_cycle_get_32();

	if (!gpio_is_ready_dt(&loopback_out) || !gpio_is_ready_dt(&loopback_in)) {
		printk("GPIO device not ready\n");
		return 0;
	}

	ret = gpio_pin_configure_dt(&loopback_out, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		printk("Failed to configure output: %d\n", ret);
		return 0;
	}

	ret = gpio_pin_configure_dt(&loopback_in, GPIO_INPUT);
	if (ret < 0) {
		printk("Failed to configure input: %d\n", ret);
		return 0;
	}

	k_sem_init(&irq_sem, 0, 1);
	k_sem_init(&sched_sem, 0, 1);
	k_timer_init(&sched_timer, sched_timer_expiry, NULL);
	tim2_setup();

	printk("PB1/PB2 benchmark start\n");
	print_startup_time();
	printk("cpu cycles per sec: %u\n", sys_clock_hw_cycles_per_sec());

	if (sample_and_check(0, "single-low") == 0) {
		pass_count++;
	} else {
		failures++;
	}

	if (sample_and_check(1, "single-high") == 0) {
		pass_count++;
	} else {
		failures++;
	}

	for (uint32_t i = 0; i < GPIO_TOGGLE_COUNT; i++) {
		int level = i & 1U;
		char tag[24];

		snprintk(tag, sizeof(tag), "toggle-%02u", i + 1U);
		if (sample_and_check(level, tag) == 0) {
			pass_count++;
		} else {
			failures++;
		}
	}

	ret = gpio_pin_set_dt(&loopback_out, 1);
	if (ret == 0) {
		k_msleep(GPIO_HOLD_MS);
		ret = gpio_pin_get_dt(&loopback_in);
		if (ret == 1) {
			printk("hold-high sample=1 PASS\n");
			pass_count++;
		} else {
			printk("hold-high sample=%d FAIL\n", ret);
			failures++;
		}
	} else {
		printk("hold-high drive failed: %d\n", ret);
		failures++;
	}

	ret = gpio_pin_set_dt(&loopback_out, 0);
	if (ret == 0) {
		k_msleep(GPIO_HOLD_MS);
		ret = gpio_pin_get_dt(&loopback_in);
		if (ret == 0) {
			printk("hold-low sample=0 PASS\n");
			pass_count++;
		} else {
			printk("hold-low sample=%d FAIL\n", ret);
			failures++;
		}
	} else {
		printk("hold-low drive failed: %d\n", ret);
		failures++;
	}

	run_irq_latency_test();
	run_sched_latency_test();
	run_ctx_switch_test();

	printk("pb_summary pass=%d fail=%d\n", pass_count, failures);
	printk("image_and_memory: see build summary for FLASH/RAM usage\n");

	return 0;
}
