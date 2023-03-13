// SPDX-License-Identifier: GPL-2.0
/*
 * ioplus_rpi Sequent Micro systems 8-Layer Stackable HAT driver
 *
 * Copyright 
 */

#include <linux/i2c.h>
#include <linux/iio/iio.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>
#include <linux/err.h>
#include <linux/mfd/core.h>

#include "ioplus_rpi.h"

static int ioplus_rpi_i2c_probe(struct i2c_client *client)
{
	struct ioplus_rpi *ioplus = devm_kmalloc(&client->dev, sizeof(*ioplus_rpi), GFP_KERNEL);
	if (!ioplus)
		return -ENOMEM;

    // add switch if other varients needed
    const struct regmap_config = &ioplus_rpi_regmap_config;
 
    i2c_set_clientdata(client, ioplus);
    ioplus->regmap = devm_regmap_init_i2c(client, regmap_config);

	if (IS_ERR(ioplus->regmap))
    {
		dev_err(&client->dev, "failed to allocate register map\n");
		return PTR_ERR(ioplus->regmap);
    }

    ioplus->client = client;

	ioplus_rpi_common_init(ioplus)

}

static void ioplus_rpi_i2c_remove(struct i2c_client *client)
{
	struct ioplus_rpi *ioplus = i2c_get_clientdata(client);

	ioplus_rpi_common_exit(ioplus);
}

static const struct of_device_id ioplus_rpi_dt_ids[] = {
	{ .compatible = "sequent,ioplus_rpi" },
	{},
};
MODULE_DEVICE_TABLE(of, ioplus_rpi_dt_ids);

static const struct i2c_device_id ioplus_rpi_i2c_id_table[] = {
	{ .compatible = "ioplus_rpi", 0},
	{}
};
MODULE_DEVICE_TABLE(i2c, ioplus_rpi_i2c_id_table);

static const struct acpi_device_id ioplus_rpi_acpi_ids[] = {
	{ .id = "ioplus_rpi", 0},
	{ }
};
MODULE_DEVICE_TABLE(acpi, rpi_i2c_acpi_ids);

static struct i2c_driver ioplus_rpi_i2c_driver = {
	.driver = {
		.name = "ioplus_rpi",
		.of_match_table = ioplus_rpi_dt_ids,
        .acpi_match_table = ioplus_rpi_acpi_ids,
	},
	.probe_new  = ioplus_rpi_i2c_probe,
    .remove		= ioplus_rpi_i2c_remove,
	.id_table   = ioplus_rpi_i2c_id_table,
};
module_i2c_driver(ioplus_rpi_i2c_driver);

MODULE_AUTHOR("");
MODULE_DESCRIPTION("Sequent Micro systems 8-Layer Stackable HAT I2C Driver");
MODULE_LICENSE("GPL v2");