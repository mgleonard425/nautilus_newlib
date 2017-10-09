NAUTILUS_DIR=/home/parallels/nautilus

AUTOMAKE_OPTIONS = cygnus
INCLUDES = -I$(NAUTILUS_DIR)/include -I$(srcdir)/include $(NEWLIB_CFLAGS) $(CROSS_CFLAGS) $(TARGET_CFLAGS)
AM_CCASFLAGS = $(INCLUDES)

noinst_LIBRARIES = lib.a

if MAY_SUPPLY_SYSCALLS
extra_objs = syscalls.o getentropy.o fs.o spinlock.o clocks.o # add more object files here if you split up
else                    						 					# syscalls.c into multiple files in the previous step
extra_objs =
endif

lib_a_SOURCES =
lib_a_LIBADD = $(extra_objs)
EXTRA_lib_a_SOURCES = syscalls.c crt0.c getentropy.c fs.c spinlock.c clocks.c # add more source files here if you split up
lib_a_DEPENDENCIES = $(extra_objs)      						 					# syscalls.c into multiple files
lib_a_CCASFLAGS = $(AM_CCASFLAGS)
lib_a_CFLAGS = $(AM_CFLAGS)

all-local: crt0.o

ACLOCAL_AMFLAGS = -I ../../.. -I ../../../..
CONFIG_STATUS_DEPENDENCIES = $(newlib_basedir)/configure.host
