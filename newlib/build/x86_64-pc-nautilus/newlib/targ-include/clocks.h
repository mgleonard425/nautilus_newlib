#include <sys/time.h>
#include <sys/times.h>
#include <sys/errno.h>
#include <sys/unistd.h>
#include <sys/io.h>

#define CLOCK_REALTIME                  0
#define CLOCK_MONOTONIC                 1
#define CLOCK_PROCESS_CPUTIME_ID        2
#define CLOCK_THREAD_CPUTIME_ID         3
#define CLOCK_MONOTONIC_RAW             4
#define CLOCK_REALTIME_COARSE           5
#define CLOCK_MONOTONIC_COARSE          6
#define CLOCK_BOOTTIME                  7
#define CLOCK_REALTIME_ALARM            8
#define CLOCK_BOOTTIME_ALARM            9
#define CLOCK_SGI_CYCLE                 10      /* Hardware specific */
#define CLOCK_TAI                       11
 
#define CMOS_ADDRESS                    0x70
#define CMOS_DATA                       0x71

#define SEC_REG                          0
#define MIN_REG                          2
#define HOUR_REG                         4
#define DAY_REG                          7
#define MONTH_REG                        8
#define YEAR_REG                         9

#define REGISTER_B                      0x0B

#define SECS_IN_MIN                      60
#define MINS_IN_HOUR                     60
#define HOURS_IN_DAY                     24
#define DAYS_IN_YEAR                     365

int get_update_in_progress_flag();
unsigned char get_RTC_register(int reg);
