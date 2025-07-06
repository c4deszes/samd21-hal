#if !defined(COMMON_SW_TIMER_H_)
#define COMMON_SW_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint16_t millis;
} swtimer_t;

/**
 * @brief Creates a software timer
 * 
 * This function allocates memory for a new software timer and initializes it.
 * 
 * @return Pointer to the created swtimer_t structure, or NULL if allocation fails.
 */
swtimer_t* SWTIMER_Create(void);

/**
 * @brief Updates all software timers by decrementing their millis value
 * 
 * This function should be called every 1 millisecond to update the timers.
 */
void SWTIMER_Update1ms();

/**
 * @brief Sets up a software timer with a specified duration
 * 
 * This function initializes the timer with a given number of milliseconds.
 * 
 * @param timer Pointer to the swtimer_t structure to be set up
 * @param millis Duration in milliseconds for the timer
 */
void SWTIMER_Setup(swtimer_t* timer, uint16_t millis);

/**
 * @brief Checks if the software timer has elapsed
 * 
 * This function checks if the timer has reached zero, indicating that the time has elapsed.
 * 
 * @param timer Pointer to the swtimer_t structure to be checked
 * @return true if the timer has elapsed, false otherwise
 */
bool SWTIMER_Elapsed(swtimer_t* timer);

#endif // COMMON_SW_TIMER_H_
