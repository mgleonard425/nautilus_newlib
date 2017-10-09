The following files were modified in newlib:

newlib/src/config.sub: Basic modifications to allow newlib to recognize nautilus as a configure option

newlib/src/newlib/configure.host: Specified directory paths and compilation flags for building newlib for nautilus

newlib/src/newlib/libc/sys/nautilus/clocks.c: A device driver for the RTC that also uses the APIC timer to get the time of day, precise to the nanosecond

newlib/src/newlib/libc/sys/nautilus/crt0.c: Basic initialization of the C runtime. This file doesn't do much.

newlib/src/newlib/libc/sys/nautilus/fs.c: Contains the syscall interface to the open file table that encapsulates most of the functionality that this port provides

newlib/src/newlib/libc/sys/nautilus/getentropy.c: An interface to getting a random number that leverages the underlying x86 architecture

newlib/src/newlib/libc/sys/nautilus/posix_memalign.c: A wrapper for newlib's libc_memalign. 

newlib/src/newlib/libc/sys/nautilus/spinlock.c: A copy of the spinlock implementation from nautilus

newlib/src/newlib/libc/sys/nautilus/syscalls.c: Stubs for the remaining syscalls necessary for newlib to link. Most of these do not make sense to be defined for nautilus. For example, many of these syscalls are process-related, such as fork, whereas nautilus has no notion of a process

newlib/src/newlib/libc/sys/nautilus/Makefile.am: To build these files

The following files were modified in nautilus:

nautilus/Makefile: To facilitate optional linking with newlib

nautilus/Kconfig: To add newlib as a menuconfig option

nautilus/.config: Added macros to allow nautilus to find newlib. THIS FILE MUST BE MODIFIED TO SPECIFY THE PATH TO THIS DIRECTORY ON YOUR MACHINE

nautilus/src/nautilus/libccompat.c: Wrapped in a macro so that this is only compiled if nautilus is not being run with newlib
