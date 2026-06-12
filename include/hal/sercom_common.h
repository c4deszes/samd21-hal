#if !defined(HAL_SERCOM_COMMON_H_)
#define HAL_SERCOM_COMMON_H_

#if defined(__cplusplus)
extern "C" {
#endif

#define SERCOM0 0
#define SERCOM1 1
#define SERCOM2 2
#define SERCOM3 3

#if defined(__ATSAMD21G__) || defined(__ATSAMD21J__)
#define SERCOM4 4
#define SERCOM5 5
#endif

void SERCOM0_Interrupt(void);

void SERCOM1_Interrupt(void);

void SERCOM2_Interrupt(void);

void SERCOM3_Interrupt(void);

#if defined(__ATSAMD21G__) || defined(__ATSAMD21J__)
void SERCOM4_Interrupt(void);

void SERCOM5_Interrupt(void);
#endif

#if defined(__cplusplus)
}
#endif

#endif // HAL_SERCOM_COMMON_H_
