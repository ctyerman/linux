// SPDX-License-Identifier: GPL-2.0
/*
 * relay driver for SequentMicrosystems ioplus-rpi
 *
 */

#include <linux/gpio/driver.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include "ioplus_rpi-core.h"


struct ioplus_rpi_relay {
	struct gpio_chip relay_chip;
	struct ioplus_rpi *ioplus_rpi;
};

static int ioplus_rpi_relay_get_direction(struct gpio_chip *gc,
				       unsigned offset)
{
	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	return GPIO_LINE_DIRECTION_OUT;
}

static int ioplus_rpi_relay_get(struct gpio_chip *gc, unsigned offset)
{
	struct ioplus_rpi_relay *relay = gpiochip_get_data(gc);
	int ret, val;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	ret = regmap_read(relay->ioplus_rpi->regmap, IOPLUS_RPI_MEM_RELAY_VAL_ADD, &val);

        printk(KERN_ALERT "return is %d, in function %s\n", ret, __func__);
        printk(KERN_ALERT "value is %d, in function %s\n", val, __func__);
        printk(KERN_ALERT "offset is %d, in function %s\n", offset, __func__);

	if (ret)
		return ret;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);


	if (val & (RELAY_STS_MASK << offset))
		return 1;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	return 0;
}

static void ioplus_rpi_relay_set(struct gpio_chip *gc, unsigned offset,
			      int value)
{
	struct ioplus_rpi_relay *relay = gpiochip_get_data(gc);

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	regmap_update_bits(relay->ioplus_rpi->regmap, IOPLUS_RPI_MEM_RELAY_VAL_ADD,
			   RELAY_SET_MASK << offset, value ? RELAY_SET_MASK : 0);
}

static const struct gpio_chip template_chip = {
	.label			= "ioplus_rpi-relay",
	.owner			= THIS_MODULE,
	.get_direction		= ioplus_rpi_relay_get_direction,
	.direction_input	= NULL,
	.direction_output	= NULL,
	.get			= ioplus_rpi_relay_get,
	.set			= ioplus_rpi_relay_set,
	.base			= -1,
	.ngpio			= 8,
	.can_sleep		= true,
};

static int ioplus_rpi_relay_probe(struct platform_device *pdev)
{
	struct ioplus_rpi *ioplus_rpi = dev_get_drvdata(pdev->dev.parent);
	struct ioplus_rpi_relay *relay;


	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	relay = devm_kzalloc(&pdev->dev, sizeof(*relay), GFP_KERNEL);
	if (!relay)
		return -ENOMEM;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);


	relay->ioplus_rpi = dev_get_drvdata(pdev->dev.parent);
	relay->relay_chip = template_chip;
	relay->relay_chip.parent = ioplus_rpi->dev;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	return devm_gpiochip_add_data(&pdev->dev, &relay->relay_chip, relay);

}

static const struct platform_device_id ioplus_rpi_relay_id_table[] = {
	{ "ioplus_rpi-relay", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(platform, ioplus_rpi_relay_id_table);

static struct platform_driver ioplus_rpi_relay_driver = {
	.driver = {
		.name = "ioplus_rpi-relay",
	},
	.probe = ioplus_rpi_relay_probe,
	.id_table = ioplus_rpi_relay_id_table,
};
module_platform_driver(ioplus_rpi_relay_driver);

MODULE_AUTHOR("");
MODULE_DESCRIPTION("ioplus_rpi relay driver");
MODULE_LICENSE("GPL v2");
