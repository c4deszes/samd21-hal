#if !defined(HAL_NVMCTRL_H_)
#define HAL_NVMCTRL_H_

#include <stdint.h>
#include <stdbool.h>

void NVMCTRL_EraseRow(uint16_t row);

void NVMCTRL_PageBufferClear(void);

void NVMCTRL_WritePage(uint32_t address);

void NVMCTRL_SetAutoPageWrite(bool enabled);

void NVMCTRL_SetReadWaitStates(uint8_t wait_states);

uint16_t NVMCTRL_GetPageSize(void);

uint16_t NVMCTRL_GetPageCount(void);

#endif // HAL_NVMCTRL_H_
