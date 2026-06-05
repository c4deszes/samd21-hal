#if !defined(HAL_SERCOM_COMMON_H_)
#define HAL_SERCOM_COMMON_H_

#if defined(__cplusplus)
extern "C" {
#endif

#define SERCOM0 0
#define SERCOM1 1
#define SERCOM2 2
#define SERCOM3 3

void SERCOM0_Interrupt(void);

void SERCOM1_Interrupt(void);

void SERCOM2_Interrupt(void);

void SERCOM3_Interrupt(void);

#if defined(__cplusplus)
}
#endif

#endif // HAL_SERCOM_COMMON_H_
