#include "devfd.h"

#include <errno.h>
#include <stdarg.h>
#include <string.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/counter.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/fdtable.h>

int zvfs_close(int fd);

#define TIMER_NODE DT_NODELABEL(timers2)
#define COUNTER_NODE DT_CHILD(TIMER_NODE, counter)
#define LOOPBACK_OUT_NODE DT_ALIAS(loopback_out)
#define LOOPBACK_IN_NODE  DT_ALIAS(loopback_in)

enum devfd_kind {
	DEVFD_KIND_GPIO,
	DEVFD_KIND_COUNTER,
};

struct devfd_obj {
	enum devfd_kind kind;
	union {
		struct gpio_dt_spec gpio;
		const struct device *counter;
	};
	bool in_use;
	int fd;
};

static int devfd_ioctl_vmeth(void *obj, unsigned int request, va_list args);
static int devfd_close_vmeth(void *obj);

static const struct fd_op_vtable devfd_vtable = {
	.read = NULL,
	.write = NULL,
	.close = devfd_close_vmeth,
	.ioctl = devfd_ioctl_vmeth,
};

static struct devfd_obj devfd_objs[3] = {
	{
		.kind = DEVFD_KIND_GPIO,
		.gpio = GPIO_DT_SPEC_GET(LOOPBACK_OUT_NODE, gpios),
	},
	{
		.kind = DEVFD_KIND_GPIO,
		.gpio = GPIO_DT_SPEC_GET(LOOPBACK_IN_NODE, gpios),
	},
	{
		.kind = DEVFD_KIND_COUNTER,
		.counter = DEVICE_DT_GET(COUNTER_NODE),
	},
};

static struct devfd_obj *devfd_find(const char *name)
{
	if (strcmp(name, "/dev/loopback_out") == 0) {
		return &devfd_objs[0];
	}

	if (strcmp(name, "/dev/loopback_in") == 0) {
		return &devfd_objs[1];
	}

	if (strcmp(name, "/dev/counter0") == 0) {
		return &devfd_objs[2];
	}

	errno = ENOENT;
	return NULL;
}

int dev_open(const char *name, int flags)
{
	struct devfd_obj *obj;
	int fd;

	ARG_UNUSED(flags);

	obj = devfd_find(name);
	if (obj == NULL) {
		return -1;
	}

	if (obj->in_use) {
		errno = EBUSY;
		return -1;
	}

	fd = zvfs_reserve_fd();
	if (fd < 0) {
		return -1;
	}

	obj->in_use = true;
	obj->fd = fd;
	zvfs_finalize_typed_fd(fd, obj, &devfd_vtable, ZVFS_MODE_IFCHR);

	return fd;
}

int dev_close(int fd)
{
	return zvfs_close(fd);
}

int dev_ioctl(int fd, unsigned long request, ...)
{
	struct devfd_obj *obj;
	va_list args;
	int ret;

	obj = zvfs_get_fd_obj(fd, &devfd_vtable, EBADF);
	if (obj == NULL) {
		return -1;
	}

	va_start(args, request);
	ret = devfd_ioctl_vmeth(obj, request, args);
	va_end(args);

	if (ret < 0) {
		errno = -ret;
		return -1;
	}

	return ret;
}

static int devfd_close_vmeth(void *obj)
{
	struct devfd_obj *devfd = obj;

	devfd->in_use = false;
	devfd->fd = -1;

	return 0;
}

static int devfd_gpio_ioctl(struct devfd_obj *obj, unsigned int request, va_list args)
{
	switch (request) {
	case GPIO_IOCTL_CONFIGURE: {
		const struct gpio_ioctl_configure *cfg = va_arg(args,
			const struct gpio_ioctl_configure *);

		return gpio_pin_configure_dt(&obj->gpio, cfg->flags);
	}

	case GPIO_IOCTL_SET: {
		const struct gpio_ioctl_set *set = va_arg(args,
			const struct gpio_ioctl_set *);

		return gpio_pin_set_dt(&obj->gpio, set->value);
	}

	case GPIO_IOCTL_GET:
		return gpio_pin_get_dt(&obj->gpio);

	default:
		return -EINVAL;
	}
}

static int devfd_counter_ioctl(struct devfd_obj *obj, unsigned int request, va_list args)
{
	switch (request) {
	case COUNTER_IOCTL_START:
		return counter_start(obj->counter);

	case COUNTER_IOCTL_GET_VALUE: {
		const struct counter_ioctl_get_value *get = va_arg(args,
			const struct counter_ioctl_get_value *);

		return counter_get_value(obj->counter, get->ticks);
	}

	case COUNTER_IOCTL_SET_ALARM: {
		const struct counter_ioctl_set_alarm *alarm = va_arg(args,
			const struct counter_ioctl_set_alarm *);

		return counter_set_channel_alarm(obj->counter, alarm->chan_id,
						 alarm->alarm_cfg);
	}

	case COUNTER_IOCTL_US_TO_TICKS: {
		const struct counter_ioctl_us_to_ticks *conv = va_arg(args,
			const struct counter_ioctl_us_to_ticks *);

		*conv->ticks = counter_us_to_ticks(obj->counter, conv->us);
		return 0;
	}

	case COUNTER_IOCTL_GET_FREQUENCY: {
		const struct counter_ioctl_get_frequency *freq = va_arg(args,
			const struct counter_ioctl_get_frequency *);

		*freq->frequency = counter_get_frequency(obj->counter);
		return 0;
	}

	default:
		return -EINVAL;
	}
}

static int devfd_ioctl_vmeth(void *obj, unsigned int request, va_list args)
{
	struct devfd_obj *devfd = obj;

	if (devfd->kind == DEVFD_KIND_GPIO) {
		return devfd_gpio_ioctl(devfd, request, args);
	}

	return devfd_counter_ioctl(devfd, request, args);
}
