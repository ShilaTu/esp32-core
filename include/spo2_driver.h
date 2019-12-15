#ifndef __SPO2_DRIVER__
#define __SPO2_DRIVER__

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define SPO2_READ_PERIOD_MS 200
#define SPO2_READ_PERIOD (SPO2_READ_PERIOD_MS / portTICK_PERIOD_MS)

#define SPO2_AGC_UP_PIN GPIO_NUM_16
#define SPO2_AGC_INC_PIN GPIO_NUM_17 

#define SPO2_PWM_RED_PIN GPIO_NUM_14
#define SPO2_PWM_IRD_PIN GPIO_NUM_27
#define SPO2_PWM_FREQUENCY 4000
#define SPO2_PWM_DUTY_16_PERCENT 675

#define SPO2_ADC_ROUNDS 4
#define SPO2_ADC_ATTEN ADC_ATTEN_DB_11
#define SPO2_ADC_RED_DC ADC1_CHANNEL_4
#define SPO2_ADC_IRD_DC ADC1_CHANNEL_5
#define SPO2_ADC_RED_AC ADC1_CHANNEL_6
#define SPO2_ADC_IRD_AC ADC1_CHANNEL_7


/**
 * spo2_init_peripherals() - initialization of necessary peripherals
 *
 * Peripherals that are initialized are 2xADC for each of the multiplexed
 * photodiode values that are preprocessed in parallel. In sum sample consists
 * of Infrared DC and AC and red DC and AC values. Additionally a 2xPWM are
 * used to be able to adjust the intensity of the LEDs. Further 2xGPIO are
 * controlling two digital potentiometers for automatic gain control of the
 * ADC inputs.
 */
void spo2_init_peripherals(void);

/**
 * spo2_read_adc() - read in a complete ADC data set
 * @data	One integer for each ADC.
 *
 * Read all ADC values that are necessary to perform calculations and save
 * each of it to data. The data is an average of SPO2_ADC_ROUNDS measurements.
 */
void spo2_read_adc(uint32_t *data);

#endif
