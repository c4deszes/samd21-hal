#if !defined(COMMON_SCH_)
#define COMMON_SCH_

/**
 * @brief Initializes the scheduler
 */
void SCH_Init(void);

/**
 * @brief Starts the scheduler
 */
void SCH_Enter(void) __attribute__ ((noreturn));

/**
 * @brief Triggers the scheduler, should be called every 1000us
 */
void SCH_Trigger(void);

void SCH_Task100us(void);

void SCH_Task1ms(void);

void SCH_Task10ms_A(void);

void SCH_Task10ms_B(void);

void SCH_Task100ms(void);

void SCH_Task500ms(void);

void SCH_Task1000ms(void);

#endif // COMMON_SCH_
