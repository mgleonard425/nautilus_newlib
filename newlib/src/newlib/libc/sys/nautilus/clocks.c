#include "include/clocks.h"

uint64_t nvram_time_since_epoch();

static uint64_t dummy_mono_clock = 0;

int get_update_in_progress_flag() {
  outb(10, CMOS_ADDRESS);
  return inb(CMOS_DATA) & 0x80;
}

unsigned char get_RTC_register(int reg) {
  outb(reg, CMOS_ADDRESS);
  return inb(CMOS_DATA);
}

int gettimeofday(struct timeval *tv, void *tz)
{
  // Second param, timezone, is just backwards compatibility. No longer used
  // This function gets the number of seconds that have passed since the epoch
  // The epoch is defined to be 0:00:00 (midnight) on January 1, 1970

  uint64_t ns_since_epoch = nvram_time_since_epoch();
  nk_vc_printf("Nanoseconds since epoch: %lu\n", ns_since_epoch);
  uint64_t us_since_epoch = ns_since_epoch/1000ULL;
  nk_vc_printf("Microseconds since epoch: %lu\n", us_since_epoch);
  tv->tv_sec = us_since_epoch / 1000000ULL;
  tv->tv_usec = us_since_epoch % 1000000ULL;
  return 0;
}

clock_t times(struct tms *buf)
{
  nk_vc_printf("Call to times ignored\n");
  return -1;
}

int clock_gettime(clockid_t clock_id, struct timespec *tp)
{
  // Should there be support for other clocks?
  if (clock_id != CLOCK_MONOTONIC) {
    printk("NAUTILUS WARNING: using invalid clock type\n");
    return -EINVAL;
  }

  if (!tp) {
    printk("NAUTILUS WARNING: using invalid timespec\n");
    return -EINVAL;
  }

#ifdef NAUT_CONFIG_HPET
  //uint64_t freq = nk_hpet_get_freq();
  uint64_t cnt  = nk_hpet_get_cntr();
  //uint64_t nsec = (1000000000/freq) * cnt;
  uint64_t nsec = cnt * nk_hpet_nanos_per_tick();
  tp->tv_sec    = nsec / 1000000000;
  tp->tv_nsec   = nsec % 1000000000;
#else
  /* runs at "10kHz" */
  tp->tv_nsec = dummy_mono_clock*100000;
  tp->tv_sec  = dummy_mono_clock/10000;
  ++dummy_mono_clock;
#endif

  return 0;
}
