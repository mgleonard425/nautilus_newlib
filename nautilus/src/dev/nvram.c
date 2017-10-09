/* 
 * This file is part of the Nautilus AeroKernel developed
 * by the Hobbes and V3VEE Projects with funding from the 
 * United States National  Science Foundation and the Department of Energy.  
 *
 * The V3VEE Project is a joint project between Northwestern University
 * and the University of New Mexico.  The Hobbes Project is a collaboration
 * led by Sandia National Laboratories that includes several national 
 * laboratories and universities. You can find out more at:
 * http://www.v3vee.org  and
 * http://xstack.sandia.gov/hobbes
 *
 * Copyright (c) 2017, Michael Leonard <michaelleonard2018@u.northwestern.edu>
 * Copyright (c) 2017, The V3VEE Project  <http://www.v3vee.org> 
 *                     The Hobbes Project <http://xstack.sandia.gov/hobbes>
 * All rights reserved.
 *
 * Author: Michael Leonard <michaelleonard2018@u.northwestern.edu>
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "LICENSE.txt".
 */

#include <nautilus/naut_types.h>
#include <nautilus/cpu.h>
#include <nautilus/dev.h>
#include <nautilus/nautilus.h>

#include <dev/apic.h>
#include <dev/nvram.h>

#ifndef NAUT_CONFIG_DEBUG_NVRAM
#undef DEBUG_PRINT
#define DEBUG_PRINT(fmt, args...) 
#endif

#define ERROR(fmt, args...) ERROR_PRINT("nvram: " fmt, ##args)
#define DEBUG(fmt, args...) DEBUG_PRINT("nvram: " fmt, ##args)
#define INFO(fmt, args...)  INFO_PRINT("nvram: " fmt, ##args)

 
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

static struct nvram_time time_at_boot;
static uint64_t cycles_at_boot;

static struct nk_dev_int ops = {
  .open=0,
  .close=0,
};

int nvram_init() {
  int rc = nvram_gettime(&time_at_boot);
  if (rc) {
    ERROR("Failed to get time from RTC\n");
    return -1;
  }

  cycles_at_boot = rdtsc();
  
  nk_dev_register("nvram",NK_DEV_GENERIC,0,&ops,0);

  INFO("Inited\n");
  return 0;
}

void nvram_deinit(){
  INFO("Deinited\n");
}

uint8_t nvram_read(uint8_t reg) {
  outb(reg, CMOS_ADDRESS);
  return inb(CMOS_DATA);
}

void nvram_write(uint8_t value, uint8_t reg) {
  outb(reg, CMOS_ADDRESS);
  outb(value, CMOS_DATA);
}

int nvram_gettime(struct nvram_time *dest) {
    // Second param, timezone, is just backwards compatibility. No longer used
  // This function gets the number of seconds that have passed since the epoch
  // The epoch is defined to be 0:00:00 (midnight) on January 1, 1970

  uint8_t BCD = !(nvram_read(REGISTER_B) & 0x04); // Set if times are in BCD, clear if in regular binary
  uint8_t twelve_hr = !(nvram_read(REGISTER_B) & 0x02); // Set if 12_hr format time instead of 24_hr format

  // All of the RTC registers hold uint8_ts
  // uint64_t is defined to be a signed long, so this should be a safe cast

  // For now, we're going to ignore the century register, and assume we're in 2x
  // The year register holds the last two digits of the year
  // So we need to add a year to account for the turn of century since 1970
  uint8_t year = nvram_read(YEAR_REG);

  uint8_t month = nvram_read(MONTH_REG);

  uint8_t day = nvram_read(DAY_REG);

  uint8_t hour = nvram_read(HOUR_REG);

  uint8_t mins = nvram_read(MIN_REG);

  uint8_t secs = nvram_read(SEC_REG);

  if (BCD) {
    secs = (secs & 0x0F) + ((secs / 16) * 10);
    mins = (mins & 0x0F) + ((mins / 16) * 10);
    hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
    day = (day & 0x0F) + ((day / 16) * 10);
    month = (month & 0x0F) + ((month / 16) * 10);
    year = (year & 0x0F) + ((year / 16) * 10);
  }

  DEBUG("Year: %ld\n", year);
  int leap_days = (year+100-70)/4 * HOURS_IN_DAY * MINS_IN_HOUR * SECS_IN_MIN;
  DEBUG("Month: %ld\n", month);
  uint64_t days_before_month; // Num days in year before first of specified month

  // This is ugly. Think about using a hash table instead
  switch(month) {
  case (1) :
    days_before_month = 0;
    break;
  case (2) :
    days_before_month = 31;
    break;
  case (3) :
    days_before_month = 59;
    break;
  case (4) :
    days_before_month = 90;
    break;
  case (5) :
    days_before_month = 120;
    break;
  case (6) :
    days_before_month = 151;
    break;
  case (7) :
    days_before_month = 181;
    break;
  case (8) :
    days_before_month = 212;
    break;
  case (9) :
    days_before_month = 243;
    break;
  case (10) :
    days_before_month = 273;
    break;
  case (11) :
    days_before_month = 304;
    break;
  case (12) :
    days_before_month = 334;
    break;
  default :
    ERROR("Got a nonsense value for month while trying to gettimeofday\n");
    return -1;
  }

  if (!(year % 4) && month > 2) {  // If this is the case, it's a leap year and february has passed
    // So we need to add one day
    ++days_before_month;
  }

  dest->unix_time = (uint64_t) ((year+100)-70)*(SECS_IN_MIN)*(MINS_IN_HOUR)*(HOURS_IN_DAY)*(DAYS_IN_YEAR);
  dest->unix_time += (uint64_t) leap_days;

  nk_vc_printf("Month: %ld\n", month);
  dest->unix_time += days_before_month * HOURS_IN_DAY * MINS_IN_HOUR * SECS_IN_MIN;

  DEBUG("Day: %ld\n", day);
  dest->unix_time += (uint64_t) day * HOURS_IN_DAY * MINS_IN_HOUR * SECS_IN_MIN;

  if (twelve_hr && (hour & 0x80)) {
    hour = ((hour & 0x7f) + 12) % 24;
  }
  DEBUG("Hour: %ld\n", hour);
  dest->unix_time += (uint64_t) hour * MINS_IN_HOUR * SECS_IN_MIN;

  DEBUG("Minute: %ld\n", mins);
  dest->unix_time += (uint64_t) mins * SECS_IN_MIN;

  DEBUG("Second: %ld\n", secs);
  dest->unix_time += (uint64_t) secs;

  dest->rtc_time.year = year;
  dest->rtc_time.month = month;
  dest->rtc_time.day = day;
  dest->rtc_time.hour = hour;
  dest->rtc_time.min = mins;
  dest->rtc_time.sec = secs;

  return 0;
}


uint64_t nvram_time_since_epoch() {
  struct sys_info *sys = per_cpu_get(system);
  struct apic_dev *apic = sys->cpus[my_cpu_id()]->apic;

  uint64_t cur_cycles = rdtsc();
  uint64_t diff_cycles = cur_cycles - cycles_at_boot;

  uint64_t ns_since_boot = apic_cycles_to_realtime(apic, diff_cycles);
  uint64_t ns_at_boot = time_at_boot.unix_time*1000000000ULL;
  DEBUG("ns_since_boot: %lu\n", ns_since_boot);
  DEBUG("ns_at_boot: %lu\n", ns_at_boot);
  DEBUG("ns since epoch: %lu\n", ns_at_boot + ns_since_boot);
  return ns_at_boot + ns_since_boot;
}
