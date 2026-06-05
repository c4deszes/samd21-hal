#include "hal/gpio.h"

void GPIO_SetupPinStandby(uint8_t group, uint8_t pin, const gpio_pin_standby_configuration* conf)
{
    /** Do nothing */
}

void GPIO_SetupPinInput(uint8_t group, uint8_t pin, const gpio_pin_input_configuration* conf)
{
    /** Do nothing */
}

/**
 * @brief Configures the pin for output operation
 * 
 * @param group Port group number eg. #PORT_GROUP_A
 * @param pin Pin number, eg. 12
 * @param conf Pointer to output configuration
 */
void GPIO_SetupPinOutput(uint8_t group, uint8_t pin, const gpio_pin_output_configuration* conf)
{
    /** Do nothing */
}

/**
 * @brief Sets the function of the given pin
 * 
 * @param group Port group number eg. #PORT_GROUP_A
 * @param pin Pin number, eg. 12
 * @param pmux 
 */
void GPIO_EnableFunction(uint8_t group, uint8_t pin, uint8_t pmux)
{
    /** Do nothing */
}

/**
 * @brief Disable the function of the given pin
 * 
 * @param group Port group number eg. #PORT_GROUP_A
 * @param pin Pin number, eg. 12
 */
void GPIO_DisableFunction(uint8_t group, uint8_t pin)
{
    /** Do nothing */
}

uint32_t GPIO_GroupRead(uint8_t group)
{
    return 0;
}

gpio_state GPIO_PinRead(uint8_t group, uint8_t pin)
{
    return LOW;
}

void GPIO_GroupWrite(uint8_t group, uint32_t value)
{
    /** Do nothing */
}

void GPIO_PinWrite(uint8_t group, uint8_t pin, gpio_state value)
{
    /** Do nothing */
}

void GPIO_PinToggle(uint8_t group, uint8_t pin)
{
    /** Do nothing */
}

void GPIO_GroupSet(uint8_t group, uint32_t mask)
{
    /** Do nothing */
}

void GPIO_GroupClear(uint8_t group, uint32_t mask)
{
    /** Do nothing */
}

void GPIO_GroupToggle(uint8_t group, uint32_t mask)
{
    /** Do nothing */
}
