#include <driver/adc.h>
#include <driver/ledc.h>
#include "driver/gpio.h"

#include "spo2_driver.h"


/**
 * spo2_init_adc() - initialize all necessary ADCs
 *
 * Alltogether four ADCs have to be initialized. All of them are initialized
 * with 3.3V ADC range. The initialized ADCs are ADC1 channels 4-7
 */
static void spo2_init_adc(void);

/**
 * spo2_init_ledc() - setup LED control.
 *
 * Setup LED Control for red and infrared LEDs on the pulsoxy sensor to 
 * operate in 4KHz PWM mode in highspeed mode.
 */
static void spo2_init_ledc(void);

/**
 * spo2_init_io() - initialize necessary GPIOs
 *
 * The digital potentiometers are using seperate pins for direction and power.
 * The UD pin for movement direction choice and the INC pin which turns the
 * actual movement on and off are setup here for the automatic gain control.
 */
static void spo2_init_io(void);

/**
 * spo2_init_dcp() - initialize digital controlled potentiometer (DCP)
 *
 * Since the CS pin is connected to GND we do never use the internal non-
 * volatile storage to write the 7-bit value of the current DCP value. Instead
 * on each startup the DCP is set down to 1kOhm initially.
 */
static void spo2_init_dcp(void);


static ledc_timer_config_t led_timer_config = {
	.timer_num = LEDC_TIMER_0,
	.speed_mode = LEDC_HIGH_SPEED_MODE,
	.freq_hz = SPO2_PWM_FREQUENCY,
	.duty_resolution = LEDC_TIMER_12_BIT
};

static ledc_channel_config_t red_channel_config = {
	.gpio_num = SPO2_PWM_RED_PIN,
	.speed_mode = LEDC_HIGH_SPEED_MODE,
	.channel = LEDC_CHANNEL_0,
	.intr_type = LEDC_INTR_DISABLE,
	.timer_sel = LEDC_TIMER_0,
	.duty = SPO2_PWM_DUTY_16_PERCENT,
	.hpoint = 0
};

static ledc_channel_config_t ird_channel_config = {
	.gpio_num = SPO2_PWM_IRD_PIN,
	.speed_mode = LEDC_HIGH_SPEED_MODE,
	.channel = LEDC_CHANNEL_1,
	.intr_type = LEDC_INTR_DISABLE,
	.timer_sel = LEDC_TIMER_0,
	.duty = SPO2_PWM_DUTY_16_PERCENT,
	.hpoint = 0
};

static gpio_config_t gpio_agc_up_config = {
	.pin_bit_mask = SPO2_DCP_UD_PIN,
	.mode = GPIO_MODE_OUTPUT,
	.pull_down_en = GPIO_PULLDOWN_ENABLE,
	.pull_up_en = GPIO_PULLUP_DISABLE,
	.intr_type = GPIO_PIN_INTR_DISABLE
};

static gpio_config_t gpio_agc_inc_config = {
	.pin_bit_mask = SPO2_DCP_INC_PIN,
	.mode = GPIO_MODE_OUTPUT,
	.pull_down_en = GPIO_PULLDOWN_ENABLE,
	.pull_up_en = GPIO_PULLUP_DISABLE,
	.intr_type = GPIO_PIN_INTR_DISABLE
};


void
spo2_init_peripherals
(void)
{
	spo2_init_adc();
	spo2_init_ledc();
	spo2_init_io();
	spo2_init_dcp();
}


static
void
spo2_init_adc
(void)
{
	ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_12));
	ESP_ERROR_CHECK(adc1_config_channel_atten(SPO2_ADC_RED_DC, SPO2_ADC_ATTEN));
	ESP_ERROR_CHECK(adc1_config_channel_atten(SPO2_ADC_IRD_DC, SPO2_ADC_ATTEN));
	ESP_ERROR_CHECK(adc1_config_channel_atten(SPO2_ADC_RED_AC, SPO2_ADC_ATTEN));
	ESP_ERROR_CHECK(adc1_config_channel_atten(SPO2_ADC_IRD_AC, SPO2_ADC_ATTEN));
	adc1_ulp_enable();
}

static
void
spo2_init_ledc
(void)
{
	ESP_ERROR_CHECK(ledc_timer_config(&led_timer_config));
	ESP_ERROR_CHECK(ledc_channel_config(&red_channel_config));
	ESP_ERROR_CHECK(ledc_channel_config(&ird_channel_config));
}

static
void
spo2_init_io
(void)
{
	ESP_ERROR_CHECK(gpio_config(&gpio_agc_up_config));
	ESP_ERROR_CHECK(gpio_config(&gpio_agc_inc_config));
}

static
void
spo2_init_dcp
(void)
{
	ESP_ERROR_CHECK(gpio_set_level(SPO2_DCP_UD_PIN, 0));
	ESP_ERROR_CHECK(gpio_set_level(SPO2_DCP_INC_PIN, 0));

	for(int i=0; i<SPO2_DCP_STEP_COUNT; i++)
	{

	}
}

