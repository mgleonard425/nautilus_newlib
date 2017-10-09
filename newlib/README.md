This directory contains the src tree, build tree, and install tree for the port of the newlib C standard library to the nautilus aerokernel.

To get information regarding newlib, see src/README

In order to build this library to ensure compatibility with nautilus, cd to the build directory and configure newlib by running the following command:

../src/configure --prefix=[newlib root] --target=x86_64-pc-nautilus --disable-multilib --disable-shared --disable-reentrant CC_FOR_TARGET=gcc AR_FOR_TARGET=ar RANLIB_FOR_TARGET=ranlib

It is recommended that the install prefix is set as the top of the newlib tree. The default prefix is /usr/local, which may break your OS.

After this, run the normal commands:

make
make install

Note: newlib does not support parallel make, so do not supply the -j flag to make
