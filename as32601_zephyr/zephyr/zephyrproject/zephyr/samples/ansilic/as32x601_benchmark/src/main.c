// #include <zephyr/device.h>
// #include <zephyr/devicetree.h>
// #include <zephyr/drivers/gpio.h>
// #include <zephyr/kernel.h>
// #include <zephyr/sys/printk.h>

// #define LED0_NODE DT_ALIAS(led0)

// #if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
// #error "Unsupported board: led0 alias is not defined"
// #endif

// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

// int main(void)
// {
// 	bool on = false;

// 	printk("AS32X601 GPIO blink test start\n");

// 	if (!gpio_is_ready_dt(&led)) {
// 		printk("led0 device not ready\n");
// 		return -ENODEV;
// 	}

// 	if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE) < 0) {
// 		printk("led0 configure failed\n");
// 		return -EIO;
// 	}

// 	printk("led0 mapped to PG15, active_low=%d\n", (led.dt_flags & GPIO_ACTIVE_LOW) ? 1 : 0);

// 	while (1) {
// 		if (gpio_pin_set_dt(&led, on ? 1 : 0) < 0) {
// 			printk("led0 set failed\n");
// 			return -EIO;
// 		}

// 		printk("PG15 set=%d\n", on ? 1 : 0);

// 		on = !on;
// 		k_msleep(1000);
// 	}
// }

#include <stdint.h>
#include "as32x601_timer.h"
#include "as32x601_plic.h"
#include "as32x601_smu.h"
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/time_units.h>

#define GPIO_TOGGLE_COUNT        20U
#define GPIO_SETTLE_MS           1U
#define GPIO_HOLD_MS             20U
#define IRQ_TEST_PERIOD_US       1000U
#define IRQ_TEST_ITERATIONS      100U
#define SCHED_TEST_ITERATIONS    100U
#define SCHED_SLEEP_US           1000U
#define CTX_SWITCH_ITERATIONS    100U
#define CTX_STACK_SIZE           512U

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

void TIM2_IRQ_Handler()
{
    if ((TIM2->SR & TIM_SR_UIF) != 0U) {
		TIM2->SR &= ~TIM_SR_UIF;
		irq_measured_cycle = k_cycle_get_32();
		irq_sample_ready = 1U;
		k_sem_give(&irq_sem);
	}
}

static void tim2_setup(void)
{
	HTIM2_CLK_ENABLE();
	PLIC_InitTypeDef PLIC_InitStructure;
	HTIM2_RESET();
	k_msleep(1);
	HTIM2_SET();


	TIM2->CR1 = 0U;
	TIM2->PSC = 90U - 1U;
	TIM2->ARR = IRQ_TEST_PERIOD_US - 1U;
	TIM2->CNT = 0U;
	TIM2->EGR = TIM_EGR_UG;
	TIM2->SR = 0U;
	TIM2->DIER = TIM_DIER_UIE;

	PLIC_StructInit(&PLIC_InitStructure);
    PLIC_InitStructure.PLIC_IRQChannel = TIM2_IRQn;
    PLIC_InitStructure.PLIC_IRQPriority = 1;
    PLIC_InitStructure.PLIC_IRQChannelCmd = ENABLE;
    PLIC_Init(&PLIC_InitStructure);
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
		HTIM2_RESET();
		k_msleep(1);
		HTIM2_SET();


		TIM2->CR1 = 0U;
		TIM2->PSC = 90U - 1U;
		TIM2->ARR = IRQ_TEST_PERIOD_US - 1U;
		TIM2->CNT = 0U;
		TIM2->EGR = TIM_EGR_UG;
		TIM2->SR = 0U;
		TIM2->DIER = TIM_DIER_UIE;
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
		//printk("sched_expected_cycle %d\n", sched_expected_cycle);
		k_timer_start(&sched_timer, K_USEC(SCHED_SLEEP_US), K_NO_WAIT);
		k_sem_take(&sched_sem, K_FOREVER);
		delta_cycles = sched_measured_cycle - sched_expected_cycle;
		printk("sched_measured_cycle %d\n", sched_measured_cycle);
		printk("sched_expected_cycle %d\n", sched_expected_cycle);

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

	k_sem_init(&irq_sem, 0, 1);
	k_sem_init(&sched_sem, 0, 1);
	k_timer_init(&sched_timer, sched_timer_expiry, NULL);
	tim2_setup();
	print_startup_time();
	printk("cpu cycles per sec: %u\n", sys_clock_hw_cycles_per_sec());

	run_irq_latency_test();
	run_sched_latency_test();
	run_ctx_switch_test();

	printk("pb_summary pass=%d fail=%d\n", pass_count, failures);
	printk("image_and_memory: see build summary for FLASH/RAM usage\n");

	return 0;
}
