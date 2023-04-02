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
#include <linux/platform_device.h>

#include "ioplus_rpi-core.h"



static bool ioplus_rpi_is_volatile_reg(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case IOPLUS_RPI_MEM_GPIO_VAL_ADD:

		return true;
	default:
		return false;
	}
}

static bool ioplus_rpi_is_writeable_reg(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case IOPLUS_RPI_MEM_GPIO_VAL_ADD:

		return true;
	default:
		return false;
	}
}


const struct regmap_config ioplus_rpi_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,

	.max_register = IOPLUS_RPI_SLAVE_BUFF_SIZE,
	.cache_type = REGCACHE_RBTREE,

	.writeable_reg = ioplus_rpi_is_writeable_reg,
	.volatile_reg = ioplus_rpi_is_volatile_reg,
};
EXPORT_SYMBOL_GPL(ioplus_rpi_regmap_config);

/*
 * MFD cells
 */
static struct mfd_cell ioplus_rpi_gpio_cells[] = {
 {	.name = "ioplus_rpi-gpio", },
 {	.name = "ioplus_rpi-relay", },
 { 	.name = "ioplus_rpi-iio", },
 { 	.name = "ioplus_rpi-pwm", },
};

/*
static int ioplus_rpi_add_device(struct ioplus_rpi *ioplus,
			       struct mfd_cell *cell)
{
	cell->platform_data = ioplus;
	cell->pdata_size = sizeof(*ioplus);

	return mfd_add_devices(&ioplus->i2c_client->dev,
			       PLATFORM_DEVID_AUTO, cell, 1, NULL, 0, NULL);
}
*/

static struct ioplus_rpi_platform_data *ioplus_rpi_parse_dt(struct device *dev)
{
	struct device_node *np = dev->of_node;
	//struct device_node *child;
	struct ioplus_rpi_platform_data *pdata;




	if (!np)
		return ERR_PTR(-EINVAL);

	pdata = devm_kzalloc(dev, sizeof(pdata), GFP_KERNEL);
	if (!pdata)
		return ERR_PTR(-ENOMEM);

	return pdata;
}

static int ioplus_rpi_startup(struct ioplus_rpi *ioplus)
{



	return 0;
}






int ioplus_rpi_common_init(struct ioplus_rpi *ioplus)
{
    struct ioplus_rpi_platform_data     *pdata;
    struct i2c_client					*client = ioplus->i2c_client;

    //struct platform_device 				*pdev;
	//struct regmap  						*regmap = ioplus->regmap;

	int ret;


    pdata = dev_get_platdata(&client->dev);


	if (!pdata)
		pdata = ioplus_rpi_parse_dt(&client->dev);
	if (IS_ERR(pdata)) {
		dev_err(&client->dev, "No platform data or DT found");
		return PTR_ERR(pdata);
	}

    ioplus->pdata = pdata;

  

    ret = ioplus_rpi_startup(ioplus);
	if (ret) {
		dev_err(&client->dev, "initialization failed\n");
		return ret;
	}

	ret = mfd_add_devices(ioplus->dev, PLATFORM_DEVID_AUTO, ioplus_rpi_gpio_cells,
			      ARRAY_SIZE(ioplus_rpi_gpio_cells), NULL, 0,
			      NULL);

	if (ret)
		return ret;







	if (ret)
		mfd_remove_devices(&client->dev);

	return ret;

}
EXPORT_SYMBOL_GPL(ioplus_rpi_common_init);

int ioplus_rpi_common_exit(struct ioplus_rpi *ioplus)
{
	mfd_remove_devices(&ioplus->i2c_client->dev);

	return 0;
}
EXPORT_SYMBOL_GPL(ioplus_rpi_common_exit);

MODULE_AUTHOR("");
MODULE_DESCRIPTION("Sequent Micro systems 8-Layer Stackable HAT Driver");
MODULE_LICENSE("GPL v2");
