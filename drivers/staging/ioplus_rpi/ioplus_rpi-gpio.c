/ SPDX-License-Identifier: GPL-2.0
/*
 * GPIO driver for SequentMicrosystems ioplus-rpi
 *
 */

#include <linux/gpio/driver.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include <linux/mfd/ioplus_rpi.h>

struct ioplus_rpi_gpio {
	struct gpio_chip gpio_chip;
	struct ioplus_rpi *ioplus_rpi;
};

static int ioplus_rpi_gpio_get_direction(struct gpio_chip *gc,
				       unsigned offset)
{
	struct ioplus_rpi_gpio *gpio = gpiochip_get_data(gc);

	int ret, val;

	ret = regmap_read(gpio->ioplus_rpi->regmap, IOPLUS_RPI_MEM_GPIO_DIR_ADD, &val);
	if (ret)
		return ret;

	if (val & GPIO_CFG_MASK << offset)
		return GPIO_LINE_DIRECTION_OUT;
	else
		return GPIO_LINE_DIRECTION_IN;
}

static int ioplus_rpi_gpio_direction_input(struct gpio_chip *gc, unsigned offset)
{
	struct ioplus_rpi_gpio *gpio = gpiochip_get_data(gc);

	return regmap_update_bits(gpio->ioplus_rpi->regmap, IOPLUS_RPI_MEM_GPIO_DIR_ADD,
				  GPIO_CFG_MASK << offset, 0);
}

static int ioplus_rpi_gpio_direction_output(struct gpio_chip *gc,
					  unsigned offset, int value)
{
	struct ioplus_rpi_gpio *gpio = gpiochip_get_data(gc);

	/* Set the initial value */
	regmap_update_bits(gpio->ioplus_rpi->regmap, IOPLUS_RPI_MEM_GPIO_VAL_ADD,
			   GPIO_SET_MASK << offset, value ? GPIO_SET_MASK : 0);

	return regmap_update_bits(gpio->ioplus_rpi->regmap, IOPLUS_RPI_MEM_GPIO_DIR_ADD,
				  GPIO_CFG_MASK << offset, GPIO_CFG_MASK);
}

static int ioplus_rpi_gpio_get(struct gpio_chip *gc, unsigned offset)
{
	struct ioplus_rpi_gpio *gpio = gpiochip_get_data(gc);
	int ret, val;

	ret = regmap_read(gpio->ioplus_rpi->regmap, IOPLUS_RPI_MEM_GPIO_VAL_ADD, &val);
	if (ret)
		return ret;

	if (val & GPIO_STS_MASK << offset)
		return 1;

	return 0;
}

static void ioplus_rpi_gpio_set(struct gpio_chip *gc, unsigned offset,
			      int value)
{
	struct ioplus_rpi_gpio *gpio = gpiochip_get_data(gc);

	regmap_update_bits(gpio->ioplus_rpi->regmap, IOPLUS_RPI_MEM_GPIO_VAL_ADD,
			   GPIO_SET_MASK << offset, value ? GPIO_SET_MASK : 0);
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
	struct ioplus_rpi *ioplus_rpi = dev_get_drvdata(pdev->dev.parent);
	struct ioplus_rpi_gpio *gpio;

	gpio = devm_kzalloc(&pdev->dev, sizeof(*gpio), GFP_KERNEL);
	if (!gpio)
		return -ENOMEM;

	gpio->ioplus_rpi = dev_get_drvdata(pdev->dev.parent);
	gpio->gpio_chip = template_chip;
	gpio->gpio_chip.parent = ioplus_rpi->dev;

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