#if !defined(COMMON_SCH_)
#define COMMON_SCH_

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initializes the scheduler
 */
void SCH_Init(void);

/**
 * @brief Starts the scheduler
 */
void SCH_Enter(void) __attribute__ ((noreturn));

/**
 * @brief Steps forward the scheduler, shouldn't be called directly unless
 *        working in environments where an infinite loop is not possible.
 *        Use SCH_Enter instead on embedded environments.
 */
void SCH_Step(void);

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

#if defined(__cplusplus)
}
#endif

#endif // COMMON_SCH_
