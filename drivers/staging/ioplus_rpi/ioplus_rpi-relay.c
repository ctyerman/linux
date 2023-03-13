/ SPDX-License-Identifier: GPL-2.0
/*
 * relay driver for SequentMicrosystems ioplus-rpi
 *
 */

#include <linux/gpio/driver.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include <linux/mfd/ioplus_rpi.h>

struct ioplus_rpi_relay {
	struct gpio_chip relay_chip;
	struct ioplus_rpi *ioplus_rpi;
};

static int ioplus_rpi_relay_get_direction(struct relay_chip *gc,
				       unsigned offset)
{
		return relay_LINE_DIRECTION_OUT;
}

static int ioplus_rpi_relay_direction_input(struct relay_chip *gc, unsigned offset)
{
	struct ioplus_rpi_relay *relay = relaychip_get_data(gc);

	return regmap_update_bits(relay->ioplus_rpi->regmap, IOPLUS_RPI_MEM_RELAY_DIR_ADD,
				  relay_CFG_MASK << offset, 0);
}

static int ioplus_rpi_relay_direction_output(struct relay_chip *gc,
					  unsigned offset, int value)
{
	struct ioplus_rpi_relay *relay = relaychip_get_data(gc);

	/* Set the initial value */
	regmap_update_bits(relay->ioplus_rpi->regmap, IOPLUS_RPI_MEM_RELAY_VAL_ADD,
			   relay_SET_MASK << offset, value ? relay_SET_MASK : 0);

	return regmap_update_bits(relay->ioplus_rpi->regmap, IOPLUS_RPI_MEM_RELAY_DIR_ADD,
				  relay_CFG_MASK << offset, relay_CFG_MASK);
}

static int ioplus_rpi_relay_get(struct relay_chip *gc, unsigned offset)
{
	struct ioplus_rpi_relay *relay = relaychip_get_data(gc);
	int ret, val;

	ret = regmap_read(relay->ioplus_rpi->regmap, IOPLUS_RPI_MEM_RELAY_VAL_ADD, &val);
	if (ret)
		return ret;

	if (val & relay_STS_MASK << offset)
		return 1;

	return 0;
}

static void ioplus_rpi_relay_set(struct relay_chip *gc, unsigned offset,
			      int value)
{
	struct ioplus_rpi_relay *relay = relaychip_get_data(gc);

	regmap_update_bits(relay->ioplus_rpi->regmap, IOPLUS_RPI_MEM_RELAY_VAL_ADD,
			   relay_SET_MASK << offset, value ? relay_SET_MASK : 0);
}

static const struct gpio_chip template_chip = {
	.label			= "ioplus_rpi-relay",
	.owner			= THIS_MODULE,
	.get_direction		= ioplus_rpi_relay_get_direction,
	.direction_input	= ioplus_rpi_relay_direction_input,
	.direction_output	= ioplus_rpi_relay_direction_output,
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

	relay = devm_kzalloc(&pdev->dev, sizeof(*relay), GFP_KERNEL);
	if (!relay)
		return -ENOMEM;

	relay->ioplus_rpi = dev_get_drvdata(pdev->dev.parent);
	relay->relay_chip = template_chip;
	relay->relay_chip.parent = ioplus_rpi->dev;

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