#ifndef DEVFD_H_
#define DEVFD_H_

#include <zephyr/drivers/counter.h>
#include <zephyr/drivers/gpio.h>

enum devfd_ioctl_request {
	GPIO_IOCTL_CONFIGURE = 0x1000,
	GPIO_IOCTL_SET,
	GPIO_IOCTL_GET,
	COUNTER_IOCTL_START = 0x2000,
	COUNTER_IOCTL_GET_VALUE,
	COUNTER_IOCTL_SET_ALARM,
	COUNTER_IOCTL_US_TO_TICKS,
	COUNTER_IOCTL_GET_FREQUENCY,
};

struct gpio_ioctl_configure {
	gpio_flags_t flags;
};

struct gpio_ioctl_set {
	int value;
};

struct counter_ioctl_get_value {
	uint32_t *ticks;
};

struct counter_ioctl_set_alarm {
	uint8_t chan_id;
	struct counter_alarm_cfg *alarm_cfg;
};

struct counter_ioctl_us_to_ticks {
	uint32_t us;
	uint32_t *ticks;
};

struct counter_ioctl_get_frequency {
	uint32_t *frequency;
};

int dev_open(const char *name, int flags);
int dev_close(int fd);
int dev_ioctl(int fd, unsigned long request, ...);

#endif
