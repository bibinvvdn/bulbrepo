#!/bin/bash

#########################################################################################
#											#
#		Build script for compat wireless driver					#
#											#
#########################################################################################

make clean

export KERNEL=`pwd`/../Kernel/ltib/rpm/BUILD/linux-3.0.35

export ARCH=arm

export CROSS_COMPILE=/opt/freescale/usr/local/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/bin/arm-none-linux-gnueabi-

./scripts/driver-select wl12xx

make

sudo find . -name "*.ko" -exec cp {} ko_ti/ \;

cp ko_ti/ ../Kernel/ltib_build_script/ -rf
