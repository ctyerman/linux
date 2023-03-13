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

struct ioplus_rpi_iio_data
{
	regmap *regmap;
	uint channel;
}


static const struct iio_chan_spec ioplus_rpi_adc_channels[] = {
	{
		.type = IIO_VOLTAGE,
		.info_mask_separate = BIT(IIO_CHAN_INFO_PROCESSED),
	},
	{
		.type = IIO_VOLTAGE,
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),
	},  
};

static const struct iio_chan_spec ioplus_rpi_quadrature_channels[] = {
	{
		.type = IIO_COUNT,
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),
	},
};

static const struct iio_chan_spec ioplus_rpi_opto_channels[] = {
	{
		.type = IIO_VOLTAGE,
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),
	},
};

static const struct iio_chan_spec ioplus_rpi_dac_channels[] = {
	{
		.type = IIO_VOLTAGE,
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW) ,
        .output = 1,
	},
};




static const struct iio_info ioplus_rpi_info = {
	.read_raw = &ioplus_rpi_read_raw,
	.write_raw = &ioplus_rpi_write_raw,
	.attrs = &ioplus_rpi_attribute_group,
};


static int ioplus_rpi_read_voltage(ioplus_rpi_iio_data data, int val);
{
	struct regmap *regmap = data->regmap;
	struct device *dev = regmap_get_device(data->regmap);
	int ret;	


	int16_t tval;
	regmap_read(regmap, IOPLUS_RPI_MEM_ADC_VAL_MV_ADD + 2* (data.ch-1) ,%tval)

	val = be32_to_cpu(tval);

	return ret;
}


static int ioplus_rpi_read_raw(struct iio_dev *indio_dev,
			   struct iio_chan_spec const *chan,
			   int *val, int *val2, long mask)
{
	struct ioplus_rpi_iio_data *data = iio_priv(indio_dev);

	switch (mask) {
	case IIO_CHAN_INFO_PROCESSED:
		switch (chan->type) {
		case IIO_VOLTAGE:
			return ioplus_rpi_read_voltage(data, val);
		default:
			return -EINVAL;
		}

	case IIO_CHAN_INFO_RAW:
		switch (chan->type) {
		case IIO_VOLTAGE:
			return ioplus_rpi_read_voltage_raw(data, val);
		case IIO_COUNT:
			return ioplus_rpi_read_count_raw(data, val);
		default:
			return -EINVAL;
		}



	default:
		return -EINVAL;
	}

}



static int ioplus_rpi_write_raw(struct iio_dev *indio_dev,
			    struct iio_chan_spec const *chan,
			    int val, int val2, long mask)
{
	struct ioplus_rpi_data *data = iio_priv(indio_dev);

	switch (mask) {
	case IIO_CHAN_INFO_PROCESSED:
		switch (chan->type) {	

		default:
			return -EINVAL;
		}

	case IIO_CHAN_INFO_RAW:
		switch (chan->type) {
		case IIO_VOLTAGE:
			if (chan.output)
				return ioplus_rpi_write_voltage_raw(data, val)

		default:
			return -EINVAL;
		}

	default:
		return -EINVAL;
	}
	

}




static int ioplus_rpi_iio_probe(struct platform_device *pdev)
{
	struct ioplus_rpi 		*ioplus = dev_get_drvdata(pdev->dev.parent);
    struct ioplus_rpi_board *pdata = dev_get_platdata(ioplus->dev);
	struct regmap 			*regmap = ioplus->regmap;

	// add adcs
	for (i = 0; i < ioplus->adc_count; i++) 
	{
		int ret;
		
		const char name[8]; 
		snprintf(name,8,"adc%d",i);
		ret = ioplus_rpi_add_iio_dev(pdev, &ioplus_rpi_adc_channels, regmap, name, i);

	}

	// add dac
	for (i = 0; i < ioplus->dac_count; i++) 
	{
		int ret;
		
		const char name[8]; 
		snprintf(name,8,"adc%d",i);
		ret = ioplus_rpi_add_iio_dev(pdev, &ioplus_rpi_dac_channels, regmap, name, i);

	}

 	// add opto
	for (i = 0; i < ioplus->opto_count; i++) 
	{
		int ret;
		
		const char name[8]; 
		snprintf(name,8,"opto%d",i);
		ret = ioplus_rpi_add_iio_dev(pdev, &ioplus_rpi_opto_channels, regmap, name, i);

	}

	// add quadrature
	for (i = 0; i < ioplus->quadrature_count; i++) 
	{
		int ret;
		
		const char name[15]; 
		snprintf(name,15,"quadrature%d",i);
		ret = ioplus_rpi_add_iio_dev(pdev, &ioplus_rpi_quadrature_channels, regmap, name, i);

	}


	return 0;
}

struct iio_dev *ioplus_rpi_add_iio_dev(struct device pdev, iio_chan_spec channels[], struct regmap *regmap, const char *name, uint ch)
{
	struct ioplus_rpi_iio_data *data;
	struct iio_dev *indio_dev;
	indio_dev = devm_iio_device_alloc(pdev->dev, sizeof(*data));
	if (!indio_dev)
		return -ERR_PTR(-ENOMEM);

    data = iio_priv(indio_dev);
	data->regmap = regmap;
	data.channel = ch;

	indio_dev->name = name;
	indio_dev->channels = channels;
	indio_dev->num_channels = ARRAY_SIZE(channels);
	indio_dev->info = &ioplus_rpi_info;
	indio_dev->modes = INDIO_DIRECT_MODE;
	
	return devm_iio_device_register(pdev->dev, indio_dev);

}

static int ioplus_rpi_iio_remove(struct platform_device *pdev)
{
	struct ioplus_rpi *ioplus;

	ioplus = dev_get_drvdata(pdev->dev.parent);



	return 0;
}

static const struct platform_device_id ioplus_rpi_iio_id_table[] = {
	{ "ioplus_rpi-iio", },
	{ /* */ }
};
MODULE_DEVICE_TABLE(platform, ioplus_rpi_iio_id_table);

static struct platform_driver ioplus_rpi_iio_driver = {
	.driver = {
		.name = "ioplus_rpi-iio",
	},
	.probe = ioplus_rpi_iio_probe,
	.remove = ioplus_rpi_iio_remove,
    .id_table = ioplus_rpi_iio_id_table,
};
module_platform_driver(ioplus_rpi_iio_driver);

MODULE_AUTHOR("");
MODULE_DESCRIPTION("Sequent Micro systems 8-Layer Stackable HAT IIO Driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:ioplus_rpi-iio");