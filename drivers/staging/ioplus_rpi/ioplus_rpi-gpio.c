// SPDX-License-Identifier: GPL-2.0
/*
 * GPIO driver for SequentMicrosystems ioplus-rpi
 *
 */

#include <linux/gpio/driver.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include "ioplus_rpi-core.h"


struct ioplus_rpi_gpio {
	struct gpio_chip gpio_chip;
	struct ioplus_rpi *ioplus_rpi;
};

static int ioplus_rpi_gpio_get_direction(struct gpio_chip *gc,
				       unsigned offset)
{
	struct ioplus_rpi_gpio *gpio = gpiochip_get_data(gc);

	int ret, val;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	//  pin not avaliable on (STM32G030C8T6) hardware version 3 
	if (gpio->ioplus_rpi->pdata->hw_major == 3 && offset == 2)
		return -ENOSYS;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	ret = regmap_read(gpio->ioplus_rpi->regmap, IOPLUS_RPI_MEM_GPIO_DIR_ADD, &val);

	printk(KERN_ALERT "return is %d, in function %s\n", ret, __func__);
	printk(KERN_ALERT "direction value %d in function %s\n", val, __func__);

	if (ret < 0)
		return ret;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);


	if (val & (GPIO_CFG_MASK << offset)) {
		printk(KERN_ALERT "direction in, in function %s\n", __func__);

		return GPIO_LINE_DIRECTION_IN;
	}
	else {
		printk(KERN_ALERT "direction out, in function %s\n", __func__);

		return GPIO_LINE_DIRECTION_OUT;
	}
}

static int ioplus_rpi_gpio_direction_input(struct gpio_chip *gc, unsigned offset)
{
	struct ioplus_rpi_gpio *gpio = gpiochip_get_data(gc);
	int ret;
	uint update_mask;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);


	update_mask = GPIO_SET_MASK << offset;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);
	printk(KERN_ALERT "\noffset is %x, in function %s\n", offset, __func__);
	printk(KERN_ALERT "mask is %x, in function %s\n\n", update_mask, __func__);

	ret = regmap_update_bits(gpio->ioplus_rpi->regmap, IOPLUS_RPI_MEM_GPIO_DIR_ADD,
				  update_mask, (GPIO_LINE_DIRECTION_IN << offset));


	if (ret < 0)
		return ret;


	printk(KERN_ALERT "return is %d, in function %s\n", ret, __func__);


	return ret;
}

static int ioplus_rpi_gpio_direction_output(struct gpio_chip *gc,
					  unsigned offset, int value)
{
	struct ioplus_rpi_gpio *gpio = gpiochip_get_data(gc);
	int ret;
	uint update_mask;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);


	//  pin not avaliable on (STM32G030C8T6) hardware version 3 
	if (gpio->ioplus_rpi->pdata->hw_major == 3 && offset == 2)
		return -ENOSYS;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	update_mask = GPIO_SET_MASK << offset;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);
	printk(KERN_ALERT "\nvalue is %x, in function %s\n", value, __func__);
	printk(KERN_ALERT "offset is %x, in function %s\n", offset, __func__);
	printk(KERN_ALERT "mask is %x, in function %s\n\n", update_mask, __func__);

	// set direction
	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	ret = regmap_update_bits(gpio->ioplus_rpi->regmap, IOPLUS_RPI_MEM_GPIO_DIR_ADD,
				  update_mask, (GPIO_LINE_DIRECTION_OUT << offset));

	printk(KERN_ALERT "return is %d, in function %s\n", ret, __func__);

	if (ret <0 )
		return ret;

	// set value
	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);
	printk(KERN_ALERT "return is %d, in function %s\n", ret, __func__);

	ret = regmap_update_bits(gpio->ioplus_rpi->regmap, IOPLUS_RPI_MEM_GPIO_VAL_ADD,
			   update_mask, ((value & GPIO_SET_MASK) << offset));

        printk(KERN_ALERT "return is %d, in function %s\n", ret, __func__);

	if (ret <0 )
		return ret;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);


	return ret;
}

static int ioplus_rpi_gpio_get(struct gpio_chip *gc, unsigned offset)
{
	struct ioplus_rpi_gpio *gpio = gpiochip_get_data(gc);
	int ret, val;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	ret = regmap_read(gpio->ioplus_rpi->regmap, IOPLUS_RPI_MEM_GPIO_VAL_ADD, &val);

	printk(KERN_ALERT "return is %d, in function %s\n", ret, __func__);

	if (ret <0 )
		return ret;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);
	printk(KERN_ALERT "value is %d, in function %s\n", val, __func__);
	printk(KERN_ALERT "offset is %d, in function %s\n", offset, __func__);

	if (val & (GPIO_STS_MASK << offset))
		return 1;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	return 0;
}

static void ioplus_rpi_gpio_set(struct gpio_chip *gc, unsigned offset,
			      int value)
{
	struct ioplus_rpi_gpio *gpio = gpiochip_get_data(gc);
	int ret;
	uint bit_mask;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	bit_mask = (GPIO_SET_MASK << offset);


	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);
	printk(KERN_ALERT "\nvalue is %d, in function %s\n", value, __func__);
	printk(KERN_ALERT "offset is %d, in function %s\n", offset, __func__);
	printk(KERN_ALERT "bitmask is %d, in function %s\n", bit_mask, __func__);

	ret = regmap_update_bits(gpio->ioplus_rpi->regmap, IOPLUS_RPI_MEM_GPIO_VAL_ADD,
			   bit_mask, ((value & GPIO_SET_MASK) << offset));

	printk(KERN_ALERT "return is %d, in function %s\n", ret, __func__);


	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);


}

static const struct gpio_chip template_chip = {
	.label			= "ioplus_rpi-gpio",
	.owner			= THIS_MODULE,
	.get_direction		= ioplus_rpi_gpio_get_direction,
	.direction_input	= ioplus_rpi_gpio_direction_input,
	.direction_output	= ioplus_rpi_gpio_direction_output,
	.get			= ioplus_rpi_gpio_get,
	.set			= ioplus_rpi_gpio_set,
	.base			= -1,
	.ngpio			= 4,
	.can_sleep		= true,
};

static int ioplus_rpi_gpio_probe(struct platform_device *pdev)
{
	struct ioplus_rpi *ioplus_rpi;
	struct ioplus_rpi_gpio *gpio;

        printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	ioplus_rpi = dev_get_drvdata(pdev->dev.parent);

        printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	gpio = devm_kzalloc(&pdev->dev, sizeof(*gpio), GFP_KERNEL);

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	if (!gpio)
		return -ENOMEM;

        printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	gpio->ioplus_rpi = dev_get_drvdata(pdev->dev.parent);

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	gpio->gpio_chip = template_chip;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	gpio->gpio_chip.parent = ioplus_rpi->dev;

	printk(KERN_ALERT "reached line %d in function %s\n", __LINE__, __func__);

	return devm_gpiochip_add_data(&pdev->dev, &gpio->gpio_chip, gpio);

}

static const struct platform_device_id ioplus_rpi_gpio_id_table[] = {
	{ "ioplus_rpi-gpio", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(platform, ioplus_rpi_gpio_id_table);

static struct platform_driver ioplus_rpi_gpio_driver = {
	.driver = {
		.name = "ioplus_rpi-gpio",
	},
	.probe = ioplus_rpi_gpio_probe,
	.id_table = ioplus_rpi_gpio_id_table,
};
module_platform_driver(ioplus_rpi_gpio_driver);

MODULE_AUTHOR("");
MODULE_DESCRIPTION("ioplus_rpi GPIO driver");
MODULE_LICENSE("GPL v2");
