	/* ADC1 channels */
	.set adc_channel_4, 5
	.set adc_channel_5, 6
	.set adc_channel_6, 7
	.set adc_channel_7, 8

	/* Number of ADC samples as log2 value */
	.set oversampling_log, 2
	.set oversampling, (1 << oversampling_log)


	.bss
	.global sample
sample:
	.fill 16


	.text
	.global entry
entry:
	MOVE r3, sample
	STAGE_RST
	MOVE r0, 0

measure_adc4:
	ADC r1, 0, adc_channel_4
	ADD r0, r0, r1

	STAGE_INC 1
	JUMPS measure_adc4, oversampling, lt

	RSH r0, r0, oversampling_log
	ST r0, r3, 0
	STAGE_RST
	MOVE r0, 0

measure_adc5:
	ADC r1, 0, adc_channel_5
	ADD r0, r0, r1

	STAGE_INC 1
	JUMPS measure_adc5, oversampling, lt

	RSH r0, r0, oversampling_log
	ST r0, r3, 4
	STAGE_RST
	MOVE r0, 0

measure_adc6:
	ADC r1, 0, adc_channel_6
	ADD r0, r0, r1

	STAGE_INC 1
	JUMPS measure_adc6, oversampling, lt

	RSH r0, r0, oversampling_log
	ST r0, r3, 8
	STAGE_RST
	MOVE r0, 0

measure_adc7:
	ADC r1, 0, adc_channel_7
	ADD r0, r0, r1

	STAGE_INC 1
	JUMPS measure_adc7, oversampling, lt

	RSH r0, r0, oversampling_log
	ST r0, r3, 12

	WAKE
	HALT
