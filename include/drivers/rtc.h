/* rtc.h - Defines for real time clock functions
 */
#ifndef _RTC_H
#define _RTC_H

#include <lib/lib.h>
#include <drivers/i8259.h>
#include <lib/math.h>

/* Constants for port selection */
#define RTC_REG_NUM_PORT 0x70
#define RTC_RW_CMOS_PORT 0x71

/* Constants for register selection and masking */
#define RTC_NMI_INIT_VAL 0x8B
#define RTC_REG_A_V_MASK 0xF0
#define RTC_REG_B_V_MASK 0x40
#define RTC_REG_A_SELECT 0x8A
#define RTC_REG_C_SELECT 0x0C

/* Init constants */
#define RTC_SILENT       0
#define RTC_VERBOSE	 	 1
#define RTC_TEST         2

/* Other constants */
#define RTC_DEFAULT_HZ   2

/* External functions */
void rtc_init(int mode);
void rtc_handle_interrupt(void);

/* External Sys Call functions */
int32_t rtc_open(void);
int32_t rtc_close(void);
int32_t rtc_read(int32_t fd, uint8_t* buf, int32_t nbytes);
int32_t rtc_write (int32_t fd, const int32_t* buf, int32_t nbytes);

#endif /* _RTC_H */
