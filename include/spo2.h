#ifndef __SPO2__
#define __SPO2__

#define SPO2_TASK_NAME "SpO2"


/**
 * spo2() - SpO2 (Pulsoxy) meassurement device interface initialization
 *
 * spo2() initializes peripherals (4xADC, 1xPWM, 2xGPIO) that is necessary
 * to operate together with the pulsoxy board which does preprocessing of input
 * data. After initialization a thread is started which periodically reads in
 * input data.
 */
void spo2(void);

#endif 
