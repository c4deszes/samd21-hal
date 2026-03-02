#include "hal/adc.h"

#include <stdlib.h>

#include "sam.h"

#define ADC_MAX_JOBS 16
adc_read_job_t* adc_job_queue[ADC_MAX_JOBS]; // Queue for pending ADC read jobs
uint8_t adc_job_queue_head = 0;
uint8_t adc_job_queue_tail = 0;
uint8_t adc_job_queue_count = 0;

/**
 * @brief Wait for the ADC registers to finish synchronizing.
 */
static void ADC_Sync() {
    while ((ADC_REGS->ADC_STATUS & ADC_STATUS_SYNCBUSY_Msk) != 0);
}

/**
 * @brief Load ADC calibration values from NVRAM.
 *
 * This function reads the ADC calibration values from the fuses and applies them to the ADC.
 * It should be called before using the ADC to ensure accurate readings.
 */
static void ADC_LoadCalibration(void) {
    // Load calibration values from NVRAM
    // This is usually done in the startup code, but can be called explicitly if needed
    // The calibration values are stored in the fuses and can be read from there
    // For example, bias and linearity calibration values can be set here
    // See the SAMD21 datasheet for more details on ADC calibration

    // Read bias and linearity calibration values from NVM fuses for ATSAMD21
    // See SAMD21 datasheet section "ADC Calibration"
    uint32_t bias = ( (*((uint32_t *)0x806020) >> 8) & 0x7 );
    uint32_t linearity = ( (*((uint32_t *)0x806020) & 0x7F) | 
                          (((*((uint32_t *)0x806020) >> 15) & 0x1) << 7) );

    ADC_Sync();

    /* Write the calibration data. */
    ADC_REGS->ADC_CALIB = ADC_CALIB_BIAS_CAL(bias) | ADC_CALIB_LINEARITY_CAL(linearity);
}

void ADC_SetupSingleShot(void) {
    ADC_Sync();
    ADC_Reset(); // Reset the ADC to ensure a clean state

    ADC_LoadCalibration();

    // Set the ADC reference voltage to VCC
    ADC_REGS->ADC_REFCTRL = ADC_REFCTRL_REFSEL_INTVCC1 | ADC_REFCTRL_REFCOMP_Msk;
    ADC_Sync();

    // Set the prescaler to 512 (1MHz / 512 = 1953.125Hz)
    ADC_REGS->ADC_CTRLB =   ADC_CTRLB_PRESCALER_DIV512 |
                            ADC_CTRLB_RESSEL_10BIT; // Set resolution to 8 bits
    ADC_REGS->ADC_SAMPCTRL = ADC_SAMPCTRL_SAMPLEN(0); // Set sample length to 0 (minimum)
    ADC_REGS->ADC_AVGCTRL = ADC_AVGCTRL_SAMPLENUM_1; // Set number of samples to 1

    ADC_REGS->ADC_INPUTCTRL = ADC_INPUTCTRL_GAIN_DIV2 | // Set gain to 1/2
                              ADC_INPUTCTRL_MUXPOS(0) | // Select input channel 0
                              ADC_INPUTCTRL_MUXNEG_GND; // Use ground as negative input
    ADC_Sync();

    // Enable the ADC
    ADC_REGS->ADC_CTRLA |= ADC_CTRLA_ENABLE_Msk;
    ADC_Sync();

    // Read the first result
    ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START_Msk; // Start the ADC
    while (!(ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk)); // Wait for the result ready flag
    (void) ADC_REGS->ADC_RESULT; // Read the result
    ADC_REGS->ADC_INTFLAG = ADC_INTFLAG_RESRDY_Msk;

    ADC_REGS->ADC_INTENSET = ADC_INTENSET_RESRDY_Msk; // Enable result ready interrupt
    NVIC_EnableIRQ(ADC_IRQn); // Enable ADC interrupt in NVIC
}

uint16_t ADC_ReadSync(uint8_t channel){
    ADC_REGS->ADC_INPUTCTRL = (ADC_REGS->ADC_INPUTCTRL & ~ADC_INPUTCTRL_MUXPOS_Msk) | ADC_INPUTCTRL_MUXPOS(channel);
    ADC_Sync();

    // Start the ADC conversion
    ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START_Msk;
    while (!(ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk)); // Wait for the result ready flag

    uint16_t result = ADC_REGS->ADC_RESULT; // Read the result
    ADC_REGS->ADC_INTFLAG = ADC_INTFLAG_RESRDY_Msk; // Clear the result ready flag

    return result;
}

static void ADC_AtomicBegin() {
    __disable_irq();
}

static void ADC_AtomicEnd() {
    __enable_irq();
}

static void ADC_PushJob(adc_read_job_t* job) {
    ADC_AtomicBegin();
    adc_job_queue[adc_job_queue_tail] = job;
    adc_job_queue_tail = (adc_job_queue_tail + 1) % ADC_MAX_JOBS;
    adc_job_queue_count++;
    ADC_AtomicEnd();
}

static adc_read_job_t* ADC_PopJob() {
    ADC_AtomicBegin();
    adc_read_job_t* job = NULL;
    if (adc_job_queue_count > 0) {
        job = adc_job_queue[adc_job_queue_head];
        adc_job_queue_head = (adc_job_queue_head + 1) % ADC_MAX_JOBS;
        adc_job_queue_count--;
    }
    ADC_AtomicEnd();
    return job;
}

void ADC_Trigger() {
    ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START_Msk; // Start the ADC
}

void ADC_SetMuxPosition(adc_read_job_t* job) {
    uint32_t inputctrl_masked = ADC_REGS->ADC_INPUTCTRL & (~ADC_INPUTCTRL_MUXPOS_Msk & ~ADC_INPUTCTRL_MUXNEG_Msk);
    ADC_REGS->ADC_INPUTCTRL = inputctrl_masked | ADC_INPUTCTRL_MUXPOS(job->muxpos) | ADC_INPUTCTRL_MUXNEG(job->muxneg);
    ADC_Sync();
}

bool ADC_ReadAsync(adc_read_job_t* job) {
    if (adc_job_queue_count >= ADC_MAX_JOBS) {
        // Queue is full, cannot add new job
        return false;
    }

    ADC_PushJob(job);

    // If this is the only job in the queue, start the ADC conversion immediately
    if (adc_job_queue_count == 1) {
        ADC_SetMuxPosition(job);
        ADC_Trigger();
    }

    return true;
}

void ADC_Handler(void) {
    if (ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk) {
        uint16_t result = ADC_REGS->ADC_RESULT; // Read the result
        ADC_REGS->ADC_INTFLAG = ADC_INTFLAG_RESRDY_Msk; // Clear the result ready flag

        adc_read_job_t* job = ADC_PopJob();
        if (job && job->callback) {
            adc_read_result_t res = { .result = result };
            job->callback(job, &res);
        }

        // Start the next ADC conversion if there are more jobs in the queue
        if (adc_job_queue_count > 0) {
            ADC_SetMuxPosition(adc_job_queue[adc_job_queue_head]);
            ADC_Trigger();
        }
    }
}

void ADC_Reset(void) {
    ADC_REGS->ADC_CTRLA = ADC_CTRLA_SWRST_Msk;
    ADC_Sync();
}

void ADC_Disable(void) {
    ADC_REGS->ADC_CTRLA &= ~ADC_CTRLA_ENABLE_Msk; // Disable the ADC
    ADC_Sync();
}
