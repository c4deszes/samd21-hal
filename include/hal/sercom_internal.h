#if !defined(HAL_SERCOM_INTERNAL_H_)
#define HAL_SERCOM_INTERNAL_H_

#include "sam.h"

sercom_registers_t* SERCOM_GetPeripheral(uint8_t sercom);

IRQn_Type SERCOM_GetInterrupt(uint8_t sercom);

#endif // HAL_SERCOM_INTERNAL_H_
