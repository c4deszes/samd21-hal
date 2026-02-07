#include "hal/nvmctrl.h"

#include "sam.h"

void NVMCTRL_EraseRow(uint32_t address) {
    
}

void NVMCTRL_PageBufferClear(void) {
    
}

void NVMCTRL_WritePage(uint32_t address) {
    
}

void NVMCTRL_SetAutoPageWrite(bool enabled) {
    
}

void NVMCTRL_SetReadWaitStates(uint8_t wait_states) {
    
}

uint16_t NVMCTRL_GetPageSize(void) {
    return 0;
}

uint16_t NVMCTRL_GetPageCount(void) {
    return 0;
}

bool NVMCTRL_WritePages(uint32_t address, const uint8_t *data, uint16_t page_count)
{
    return true;
}