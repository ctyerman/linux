struct ioplus_rpi_pwm {
	struct pwm_chip pwm_chip;
	struct ioplus_rpi *ioplus_rpi;

};


static const struct ioplus_rpi_pwm_ops = {
	.request = ioplus_rpi_pwm_request,
	.free = ioplus_rpi_pwm_free,
	.apply = ioplus_rpi_pwm_apply,
	.owner = THIS_MODULE,


}

static int ioplus_rpi_pwm_probe(struct platform_device *pdev)
{
	struct ioplus_rpi *ioplus_rpi = dev_get_drvdata(pdev->dev.parent);
	struct ioplus_rpi_pwm *pwm;
    int ret = 0 

	pwm = devm_kzalloc(&pdev->dev, sizeof(*pwm), GFP_KERNEL);
	if (!pwm)
		return -ENOMEM;

	pwm->ioplus_rpi = dev_get_drvdata(pdev->dev.parent);
	
	pwm->pwm_chip.dev = &pdev->dev;
	pwm->pwm_chip.ops = &ioplus_rpi_pwm_ops;
	pwm->pwm_chip.npwm = 4;    

    platform_set_drvdata(pdev, pwm);

    ret = devm_pwmchip_add(&pdev->dev,*pwm->pwm_chip);
	if (ret < 0) {
		dev_err(&pdev->dev, "pwmchip_add() failed: %d\n", ret);
		return ret;
	}

    return 0;
}


static const struct platform_device_id ioplus_rpi_pwm_id_table[] = {
	{ "ioplus_rpi-pwm", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(platform, ioplus_rpi_pwm_id_table);

static struct platform_driver ioplus_rpi_pwm_driver = {
	.driver = {
		.name = "ioplus_rpi-pwm",
	},
	.probe = ioplus_rpi_pwm_probe,
	.id_table = ioplus_rpi_pwm_id_table,
};
module_platform_driver(ioplus_rpi_pwm_driver);

MODULE_AUTHOR("");
MODULE_DESCRIPTION("ioplus_rpi PWM driver");
MODULE_LICENSE("GPL v2");