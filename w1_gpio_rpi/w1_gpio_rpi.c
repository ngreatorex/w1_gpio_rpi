#include <linux/module.h>
#include <linux/init.h>
#include <linux/w1-gpio.h>
#include <linux/platform_device.h>

#define VERSION "0.1"

static unsigned int w1_gpio_pin = 4;

module_param(w1_gpio_pin, uint, 0444);
MODULE_PARM_DESC(w1_gpio_pin, "GPIO pin to use for 1-wire data");

static struct w1_gpio_platform_data pdata = {
	.pin = 0,
	.is_open_drain = 0,
};

static void w1_gpio_rpi_release(struct device *dev)
{
	/* nothing to do */
}

static struct platform_device pdev = {
	.name = "w1-gpio",
	.id = -1,
	.dev = {
		.platform_data = &pdata,
		.release = w1_gpio_rpi_release,
	},
}; 


static int __init w1_gpio_rpi_init(void)
{
	int err;

	printk("Raspberry Pi 1-wire GPIO " VERSION ": initialising on pin %d\n", w1_gpio_pin);

	pdata.pin = w1_gpio_pin;
	
	err = platform_device_register(&pdev);
	if (err) 
		goto err;

	printk("Raspberry Pi 1-wire GPIO" VERSION ": platform device registered\n");
	return 0;

err:
	return err;
}

static void __exit w1_gpio_rpi_exit(void)
{
	printk("Raspberry Pi 1-wire GPIO" VERSION ": unloading\n");

	platform_device_unregister(&pdev);

	return;
}

module_init(w1_gpio_rpi_init);
module_exit(w1_gpio_rpi_exit);

MODULE_AUTHOR("Neil Greatorex <neil@fatboyfat.co.uk>");
MODULE_DESCRIPTION("Dallas 1-wire over GPIO for Raspberry Pi");
MODULE_VERSION(VERSION);
MODULE_LICENSE("GPL");

