#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/gpio.h>

#define UART_NODE DT_NODELABEL(uart5)
#define DE_NODE   DT_PATH(rs485_ctrl, rs485_de_0)
#define RX_BUF_SIZE 64
#define RX_IDLE_TIMEOUT_MS 5
#define RX_POLL_DELAY_US 5
#define DE_ASSERT_DELAY_US 200

static const struct device *uart_dev = DEVICE_DT_GET(UART_NODE);
static const struct gpio_dt_spec de_pin = GPIO_DT_SPEC_GET(DE_NODE, gpios);

static void print_buffer(const char *label, const uint8_t *buf, size_t len)
{
	printk("%s", label);
	for (size_t i = 0; i < len; i++) {
		printk(" %02x", buf[i]);
	}
	printk("\n");
}

int main(void)
{
	uint8_t rx_buf[RX_BUF_SIZE];
	size_t rx_len = 0U;
	int64_t last_rx_time = 0;

	if (!device_is_ready(uart_dev)) {
		printk("UART device not ready\n");
		return -1;
	}
	if (!device_is_ready(de_pin.port)) {
		printk("GPIO device not ready\n");
		return -1;
	}

	gpio_pin_configure_dt(&de_pin, GPIO_OUTPUT_INACTIVE);
	printk("RS485 echo test ready\n");

	while (1) {
		unsigned char c;

		if (uart_poll_in(uart_dev, &c) == 0) {
			if (rx_len < sizeof(rx_buf)) {
				rx_buf[rx_len++] = c;
			}
			last_rx_time = k_uptime_get();
			continue;
		}

		if ((rx_len > 0U) &&
		    ((rx_len == sizeof(rx_buf)) ||
		     (k_uptime_get() - last_rx_time >= RX_IDLE_TIMEOUT_MS))) {
			print_buffer("RX:", rx_buf, rx_len);

			gpio_pin_set_dt(&de_pin, 1);
			k_busy_wait(DE_ASSERT_DELAY_US);
			for (size_t i = 0; i < rx_len; i++) {
				uart_poll_out(uart_dev, rx_buf[i]);
			}
			gpio_pin_set_dt(&de_pin, 0);

			print_buffer("TX:", rx_buf, rx_len);
			rx_len = 0U;
		}

		k_busy_wait(RX_POLL_DELAY_US);
	}

	return 0;
}
