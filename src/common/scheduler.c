#include "common/scheduler.h"

#include <stdint.h>

static volatile uint8_t task1ms_flag = 0;
static volatile uint8_t task10ms_flag = 0;
static volatile uint8_t task100ms_flag = 0;
static volatile uint8_t task500ms_flag = 0;
static volatile uint8_t task1000ms_flag = 0;

void SCH_Init(void) {
    task1ms_flag = 0;
    task10ms_flag = 0;
    task100ms_flag = 0;
    task500ms_flag = 0;
    task1000ms_flag = 0;
}

void SCH_Enter(void) {
    while(1) {
        if (task1ms_flag) {
            SCH_Task1ms();
            task1ms_flag = 0;
            task10ms_flag++;
        }
        if (task10ms_flag >= 10) {
            SCH_Task10ms_A();
            task10ms_flag = 0;
            task100ms_flag++;
        }
        if (task100ms_flag >= 10) {
            SCH_Task100ms();
            task100ms_flag = 0;
            task500ms_flag++;
        }
        if (task500ms_flag >= 5) {
            SCH_Task500ms();
            task500ms_flag = 0;
            task1000ms_flag++;
        }
        if (task1000ms_flag >= 2) {
            SCH_Task1000ms();
            task1000ms_flag = 0;
        }
    }
}

void SCH_Trigger(void) {
    task1ms_flag = 1;
}

static void SCH_DummyTask(void) {
    /** Do nothing */
}

void SCH_Task1ms(void) __attribute__ ((weak, alias("SCH_DummyTask")));

void SCH_Task10ms_A(void) __attribute__ ((weak, alias("SCH_DummyTask")));

void SCH_Task10ms_B(void) __attribute__ ((weak, alias("SCH_DummyTask")));

void SCH_Task100ms(void) __attribute__ ((weak, alias("SCH_DummyTask")));

void SCH_Task500ms(void) __attribute__ ((weak, alias("SCH_DummyTask")));

void SCH_Task1000ms(void) __attribute__ ((weak, alias("SCH_DummyTask")));
