#include <stdint.h>
#include <stm32f4xx.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
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

#define TEST_PERIOD_US  1000U
#define TEST_ITERATIONS 100U
#define REPORT_EVERY    10U

static const struct gpio_dt_spec loopback_out = GPIO_DT_SPEC_GET(LOOPBACK_OUT_NODE, gpios);
static const struct gpio_dt_spec loopback_in = GPIO_DT_SPEC_GET(LOOPBACK_IN_NODE, gpios);
static struct k_sem sample_sem;

static volatile uint32_t expected_cycle;
static volatile uint32_t measured_cycle;
static volatile uint32_t sample_ready;

static uint32_t cycles_to_ns_u32(uint32_t cycles)
{
	uint64_t ns = k_cyc_to_ns_floor64(cycles);

	if (ns > UINT32_MAX) {
		return UINT32_MAX;
	}

	return (uint32_t)ns;
}

static int run_gpio_check(int level)
{
	int ret;

	ret = gpio_pin_set_dt(&loopback_out, level);
	if (ret < 0) {
		printk("Failed to drive output: %d\n", ret);
		return ret;
	}

	k_msleep(1);

	ret = gpio_pin_get_dt(&loopback_in);
	if (ret < 0) {
		printk("Failed to sample input: %d\n", ret);
		return ret;
	}

	printk("PB1->PB2 drive=%d sample=%d %s\n",
	       level, ret, (ret == level) ? "PASS" : "FAIL");

	return (ret == level) ? 0 : -EIO;
}

static void tim2_direct_isr(const void *arg)
{
	ARG_UNUSED(arg);

	if ((TIM2->SR & TIM_SR_UIF) != 0U) {
		TIM2->SR &= ~TIM_SR_UIF;
		measured_cycle = k_cycle_get_32();
		sample_ready = 1U;
		k_sem_give(&sample_sem);
	}
}

static void tim2_setup(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	__DSB();

	TIM2->CR1 = 0U;
	TIM2->PSC = 84U - 1U;
	TIM2->ARR = TEST_PERIOD_US - 1U;
	TIM2->CNT = 0U;
	TIM2->EGR = TIM_EGR_UG;
	TIM2->SR = 0U;
	TIM2->DIER = TIM_DIER_UIE;

	IRQ_CONNECT(TIM2_IRQn, 0, tim2_direct_isr, NULL, 0);
	irq_enable(TIM2_IRQn);
}

int main(void)
{
	uint32_t min_cycles = UINT32_MAX;
	uint32_t max_cycles = 0U;
	uint64_t sum_cycles = 0U;
	int ret;

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

	k_sem_init(&sample_sem, 0, 1);
	tim2_setup();

	printk("direct TIM2 IRQ handler latency + PB1/PB2 test\n");
	printk("cpu cycles per sec: %u\n", sys_clock_hw_cycles_per_sec());
	printk("test period: %u us\n", TEST_PERIOD_US);
	printk("iterations: %u\n", TEST_ITERATIONS);

	run_gpio_check(0);
	run_gpio_check(1);

	for (uint32_t i = 1; i <= TEST_ITERATIONS; i++) {
		uint32_t delta_cycles;

		sample_ready = 0U;
		TIM2->CNT = 0U;
		TIM2->SR = 0U;
		expected_cycle = k_cycle_get_32() + k_us_to_cyc_ceil32(TEST_PERIOD_US);
		TIM2->CR1 |= TIM_CR1_CEN;

		k_sem_take(&sample_sem, K_FOREVER);
		TIM2->CR1 &= ~TIM_CR1_CEN;

		if (!sample_ready) {
			printk("sample not ready\n");
			break;
		}

		delta_cycles = measured_cycle - expected_cycle;
		if (delta_cycles < min_cycles) {
			min_cycles = delta_cycles;
		}
		if (delta_cycles > max_cycles) {
			max_cycles = delta_cycles;
		}
		sum_cycles += delta_cycles;

		if ((i % REPORT_EVERY) == 0U) {
			uint32_t avg_cycles = (uint32_t)(sum_cycles / i);

			printk("[%u] cur=%u cyc (%u ns) min=%u max=%u avg=%u\n",
			       i,
			       delta_cycles,
			       cycles_to_ns_u32(delta_cycles),
			       min_cycles,
			       max_cycles,
			       avg_cycles);
		}
	}

	printk("done: min=%u cyc (%u ns), max=%u cyc (%u ns), avg=%u cyc (%u ns)\n",
	       min_cycles,
	       cycles_to_ns_u32(min_cycles),
	       max_cycles,
	       cycles_to_ns_u32(max_cycles),
	       (uint32_t)(sum_cycles / TEST_ITERATIONS),
	       cycles_to_ns_u32((uint32_t)(sum_cycles / TEST_ITERATIONS)));

	return 0;
}
