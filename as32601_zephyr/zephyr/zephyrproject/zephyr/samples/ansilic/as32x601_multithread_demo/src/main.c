#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <stdint.h>

#define LED0_NODE DT_ALIAS(led0)

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 alias is not defined"
#endif

#define WORKER_A_STACK_SIZE 2048
#define WORKER_B_STACK_SIZE 2048
#define WORKER_A_PRIORITY -1
#define WORKER_B_PRIORITY -1
#define WORKER_A_PERIOD_MS 100
#define WORKER_B_PERIOD_MS 250
#define STATUS_PERIOD_MS 1000

K_THREAD_STACK_DEFINE(worker_a_stack, WORKER_A_STACK_SIZE);
K_THREAD_STACK_DEFINE(worker_b_stack, WORKER_B_STACK_SIZE);
static struct k_thread worker_a_thread_data;
static struct k_thread worker_b_thread_data;

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

extern volatile uint32_t as32x601_timer_isr_count;
extern volatile uint32_t as32x601_sys_clock_announce_count;
extern volatile uint32_t as32x601_sleep_enter_count;
extern volatile uint32_t as32x601_thread_timeout_count;
extern volatile uint32_t as32x601_ready_thread_count;

static volatile uint32_t worker_a_count;
static volatile uint32_t worker_b_count;
static volatile uint32_t worker_a_wake_count;
static volatile uint32_t worker_b_wake_count;

static void worker_a(void *arg1, void *arg2, void *arg3)
{
	ARG_UNUSED(arg1);
	ARG_UNUSED(arg2);
	ARG_UNUSED(arg3);

	while (1) {
		worker_a_count++;
		k_msleep(WORKER_A_PERIOD_MS);
		worker_a_wake_count++;
	}
}

static void worker_b(void *arg1, void *arg2, void *arg3)
{
	ARG_UNUSED(arg1);
	ARG_UNUSED(arg2);
	ARG_UNUSED(arg3);

	while (1) {
		worker_b_count++;
		k_msleep(WORKER_B_PERIOD_MS);
		worker_b_wake_count++;
	}
}

int main(void)
{
	bool led_on = false;

	printk("multithread sleep probe start\n");

	if (!gpio_is_ready_dt(&led)) {
		printk("led0 device not ready\n");
		return -ENODEV;
	}

	if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE) < 0) {
		printk("led0 configure failed\n");
		return -EIO;
	}

	printk("led0 ready, active_low=%d\n",
	       (led.dt_flags & GPIO_ACTIVE_LOW) ? 1 : 0);

	k_thread_create(&worker_a_thread_data,
				worker_a_stack,
				K_THREAD_STACK_SIZEOF(worker_a_stack),
				worker_a,
				NULL, NULL, NULL,
				WORKER_A_PRIORITY,
				0,
				K_NO_WAIT);

	k_thread_create(&worker_b_thread_data,
				worker_b_stack,
				K_THREAD_STACK_SIZEOF(worker_b_stack),
				worker_b,
				NULL, NULL, NULL,
				WORKER_B_PRIORITY,
				0,
				K_NO_WAIT);

	while (1) {
		if (gpio_pin_set_dt(&led, led_on ? 1 : 0) < 0) {
			printk("led0 set failed\n");
			return -EIO;
		}

		printk("up=%lld led=%d tisr=%u announce=%u a=%u/%u b=%u/%u sleep=%u timeout=%u ready=%u\n",
		       (long long)k_uptime_get(),
		       led_on ? 1 : 0,
		       as32x601_timer_isr_count,
		       as32x601_sys_clock_announce_count,
		       worker_a_count,
		       worker_a_wake_count,
		       worker_b_count,
		       worker_b_wake_count,
		       as32x601_sleep_enter_count,
		       as32x601_thread_timeout_count,
		       as32x601_ready_thread_count);

		led_on = !led_on;
		k_msleep(STATUS_PERIOD_MS);
	}
}
