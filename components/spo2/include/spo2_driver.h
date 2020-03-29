#ifndef __SPO2_DRIVER__
#define __SPO2_DRIVER__

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define SPO2_READ_PERIOD_MS 200
#define SPO2_READ_PERIOD (SPO2_READ_PERIOD_MS / portTICK_PERIOD_MS)

#define SPO2_DCP_STEP_COUNT 100
#define SPO2_DCP_UD_PIN GPIO_NUM_16
#define SPO2_DCP_INC_PIN GPIO_NUM_17 

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
 * struct spo2_adc_sample - life-sensor spo2 adc sample struct
 * @red_dc:	red LED DC value
 * @ird_dc:	infrared LED DC value
 * @red_ac:	red LED AC value
 * @ird_ac:	infrared LED AC value
 *
 * This struct saves an ADC sample necessary for the spo2 calculations.
 * NOTE: Only the lower 16 bit are relevant.
 */
typedef struct __attribute__((packed)) {
	uint32_t red_dc;
	uint32_t ird_dc;
	uint32_t red_ac;
	uint32_t ird_ac;
} _spo2_adc_sample;


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
 * @sample	single set of ADC readings
 *
 * Read all ADC values that are necessary to perform calculations and save
 * each of it to data. The data is an average of SPO2_ADC_ROUNDS measurements.
 */
void spo2_read_adc(_spo2_adc_sample *sample);

#endif

