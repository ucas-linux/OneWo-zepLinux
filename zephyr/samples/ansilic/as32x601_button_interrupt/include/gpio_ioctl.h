#ifndef GPIO_IOCTL_H_
#define GPIO_IOCTL_H_

#include <zephyr/drivers/gpio.h>

#define _IOC(dir, type, nr, size) \
	(((unsigned long)(dir)  << 30) | \
	 ((unsigned long)(size) << 16) | \
	 ((unsigned long)(type) << 8)  | \
	 ((unsigned long)(nr)   << 0))

#define _IOC_NONE  0UL
#define _IOC_WRITE 1UL
#define _IOC_READ  2UL

#define _IOW(type, nr, t)  _IOC(_IOC_WRITE,            (type), (nr), sizeof(t))
#define _IOWR(type, nr, t) _IOC(_IOC_READ | _IOC_WRITE, (type), (nr), sizeof(t))

#define GPIO_IOC_MAGIC        'G'
#define GPIO_IOC_CONFIGURE    _IOW(GPIO_IOC_MAGIC, 0x01, struct gpio_ioctl_cfg)
#define GPIO_IOC_SET_IRQ      _IOW(GPIO_IOC_MAGIC, 0x02, struct gpio_ioctl_irq)
#define GPIO_IOC_GET_VALUE    _IOWR(GPIO_IOC_MAGIC, 0x03, struct gpio_ioctl_value)

struct gpio_ioctl_cfg {
	gpio_pin_t pin;
	gpio_flags_t flags;
};

struct gpio_ioctl_irq {
	gpio_pin_t pin;
	gpio_flags_t flags;
};

struct gpio_ioctl_value {
	gpio_pin_t pin;
	int value;
};

#endif
