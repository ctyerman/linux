/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/bitops.h>
#include <linux/device.h>
#include <linux/regmap.h>
#include <linux/i2c.h>

/* Regmap configurations */
extern const struct regmap_config ioplus_rpi_regmap_config;


/* Probe called from different transports */
int ioplus_rpi_common_probe(struct ioplus_rpi *ioplus);


/* */
#define ADC_CH_NO	            8
#define DAC_CH_NO	            4
#define ADC_RAW_VAL_SIZE	    2
#define DAC_MV_VAL_SIZE		    2
#define VOLT_TO_MILIVOLT	    1000
#define OPTO_CH_NO              8
#define GPIO_CH_NO              4
#define COUNTER_SIZE            4

#define RETRY_TIMES             10
#define CALIBRATION_KEY         0xaa
#define RESET_CALIBRATION_KEY   0x55 
#define WDT_RESET_SIGNATURE     0xCA
#define WDT_MAX_OFF_INTERVAL_S  4147200 //48 days


#define GPIO_CFG_MASK			0x01
#define GPIO_STS_MASK			0x01
#define GPIO_SET_MASK			0x01




/* specific registers */
#define IOPLUS_RPI_MEM_RELAY_VAL_ADD            0x00
#define IOPLUS_RPI_MEM_RELAY_SET_ADD            0x01
#define IOPLUS_RPI_MEM_RELAY_CLR_ADD            0x02
#define IOPLUS_RPI_MEM_OPTO_IN_ADD              0x03
#define IOPLUS_RPI_MEM_GPIO_VAL_ADD             0x04  // 8 channels per byte GPIO4|GPIO3|GPIO2|GPIO1
#define IOPLUS_RPI_MEM_GPIO_SET_ADD             0x05  //
#define IOPLUS_RPI_MEM_GPIO_CLR_ADD             0x06
#define IOPLUS_RPI_MEM_GPIO_DIR_ADD             0x07  // 

#define IOPLUS_RPI_MEM_ADC_VAL_RAW_ADD          0x08
#define IOPLUS_RPI_MEM_ADC_VAL_MV_ADD           0x18
#define IOPLUS_RPI_MEM_DAC_VAL_MV_ADD           0x28
#define IOPLUS_RPI_MEM_OD_PWM_VAL_RAW_ADD       0x30
#define IOPLUS_RPI_MEM_OPTO_IT_RISING_ADD       0x38
#define IOPLUS_RPI_MEM_OPTO_IT_FALLING_ADD      0x39
#define IOPLUS_RPI_MEM_GPIO_EXT_IT_RISING_ADD   0x40
#define IOPLUS_RPI_MEM_GPIO_EXT_IT_FALLING_ADD  0x41
#define IOPLUS_RPI_MEM_OPTO_CNT_RST_ADD         0x42
#define IOPLUS_RPI_MEM_GPIO_CNT_RST_ADD         0x43

#define IOPLUS_RPI_MEM_DIAG_TEMPERATURE_ADD     0x44

#define IOPLUS_RPI_MEM_DIAG_3V3_MV_ADD          0x45
#define IOPLUS_RPI_MEM_DIAG_3V3_MV_ADD1         0x46

#define IOPLUS_RPI_MEM_CALIB_VALUE              0x47
#define IOPLUS_RPI_MEM_CALIB_CHANNEL            0x49 // ADC channels [1,8]; DAC channels [9, 12]
#define IOPLUS_RPI_MEM_CALIB_KEY                0x50 // set calib point 0xaa; reset calibration on the channel 0x55
#define IOPLUS_RPI_MEM_CALIB_STATUS             0x51

#define IOPLUS_RPI_MEM_OPTO_ENC_ENABLE_ADD      0x52
#define IOPLUS_RPI_MEM_GPIO_ENC_ENABLE_ADD      0x53
#define IOPLUS_RPI_MEM_OPTO_ENC_CNT_RST_ADD     0x54
#define IOPLUS_RPI_MEM_GPIO_ENC_CNT_RST_ADD     0x55

#define IOPLUS_RPI_MEM_WDT_RESET_ADD                0x64
#define IOPLUS_RPI_MEM_WDT_INTERVAL_SET_ADD         0x65
#define IOPLUS_RPI_MEM_WDT_INTERVAL_GET_ADD         0x67
#define IOPLUS_RPI_MEM_WDT_INIT_INTERVAL_SET_ADD    0x69
#define IOPLUS_RPI_MEM_WDT_INIT_INTERVAL_GET_ADD    0x6b
#define IOPLUS_RPI_MEM_WDT_RESET_COUNT_ADD          0x6d
#define IOPLUS_RPI_MEM_WDT_CLEAR_RESET_COUNT_ADD    0x6f

#define IOPLUS_RPI_MEM_WDT_POWER_OFF_INTERVAL_SET_ADD   0x70
#define IOPLUS_RPI_MEM_WDT_POWER_OFF_INTERVAL_GET_ADD   0x74

#define IOPLUS_RPI_MEM_REVISION_HW_MAJOR_ADD    0x78
#define IOPLUS_RPI_MEM_REVISION_HW_MINOR_ADD    0x79
#define IOPLUS_RPI_MEM_REVISION_MAJOR_ADD       0x7a
#define IOPLUS_RPI_MEM_REVISION_MINOR_ADD       0x7b
#define IOPLUS_RPI_DBG_FIFO_SIZE                0x7c
#define IOPLUS_RPI_DBG_FIFO_ADD                 0x7e
#define IOPLUS_RPI_DBG_CMD                      0x7f
#define IOPLUS_RPI_MEM_OPTO_EDGE_COUNT_ADD      0x80
#define IOPLUS_RPI_MEM_OPTO_EDGE_COUNT_END_ADD  0xa0
#define IOPLUS_RPI_MEM_OD_PWM_FREQUENCY         0xa1
#define IOPLUS_RPI_MEM_ADD_RESERVED             0xaa
#define IOPLUS_RPI_MEM_GPIO_EDGE_COUNT_ADD      0xab
#define IOPLUS_RPI_MEM_OPTO_ENC_COUNT_ADD       0xbb
#define IOPLUS_RPI_MEM_GPIO_ENC_COUNT_ADD       0xcb
#define IOPLUS_RPI_MEM_GPIO_ENC_COUNT_END_ADD   0xd3
#define IOPLUS_RPI_SLAVE_BUFF_SIZE              0xd4


struct ioplus_rpi {
	struct device *dev;
	struct i2c_client *i2c_client;
	struct regmap *regmap;

    uint adc_count;    
    uint dac_count;
    uint opto_count;
    uint quadrature_count;

    uint gpio_count;
    uint relay_count;
    uint pwm_count;


}


#endif  /* IOPLUS_RPI_H_ */
