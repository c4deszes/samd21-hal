#if !defined(HAL_NVMCTRL_H_)
#define HAL_NVMCTRL_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Erase a row in the Non-Volatile Memory Controller (NVMCTRL).
 * 
 * @param address Row address to erase. The address should be aligned to the row size.
 */
void NVMCTRL_EraseRow(uint32_t address);

/**
 * @brief Clear the page buffer in the NVMCTRL.
 * 
 * This function clears the page buffer, preparing it for a new write operation.
 */
void NVMCTRL_PageBufferClear(void);

/**
 * @brief Write a page to the NVMCTRL, this writes the contents of the page buffer to the specified
 * address.
 * 
 * @param address Address of the page to write. The address should be aligned to the page size.
 */
void NVMCTRL_WritePage(uint32_t address);

/**
 * @brief Enable or disable automatic page write in the NVMCTRL. If enabled the NVMCTRL will
 * automatically write the page buffer to the specified address when the page buffer is full.
 * 
 * @param enabled If true, automatic page write is enabled; otherwise, it is disabled.
 */
void NVMCTRL_SetAutoPageWrite(bool enabled);

void NVMCTRL_SetReadWaitStates(uint8_t wait_states);

/**
 * @brief Get the size of a page in the NVMCTRL. The result will be 64 bytes for most SAM devices.
 * 
 * @return The size of a page in bytes.
 */
uint16_t NVMCTRL_GetPageSize(void);

/**
 * @brief Get the total number of pages in the NVMCTRL. The result will depend on the specific
 * device and its memory configuration.
 * 
 * @return The total number of pages.
 */
uint16_t NVMCTRL_GetPageCount(void);

/**
 * @brief Write multiple pages to the NVMCTRL.
 * 
 * This function writes the specified number of pages to the NVMCTRL starting from the given address.
 * The data is expected to be in a contiguous buffer, with each page being 64 bytes.
 * 
 * @param address Starting address to write the pages. The address should be aligned to the page size.
 * @param data Pointer to the data buffer containing the pages to write.
 * @param page_count Number of pages to write.
 * @return true if the operation was successful, false otherwise.
 */
bool NVMCTRL_WritePages(uint32_t address, const uint8_t *data, uint16_t page_count);

#endif // HAL_NVMCTRL_H_
