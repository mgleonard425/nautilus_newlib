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
#ifndef __NVRAM_H__
#define __NVRAM_H__

#include <nautilus/naut_types.h>

int nvram_init();
void nvram_deinit();

uint8_t nvram_read(uint8_t reg);
void nvram_write(uint8_t value, uint8_t reg);

struct nvram_time {
  uint64_t unix_time;  // Time in seconds since epoch
  struct {
    uint8_t sec, min, hour, day, month, year; // Binary representation, NOT BCD
                                              // Only gives 24-hr format, never 12-hr
  } rtc_time;
};

int nvram_gettime(struct nvram_time *dest);

uint64_t nvram_time_since_epoch(); // Returns time in nanoseconds

#endif   // __NVRAM_H__
