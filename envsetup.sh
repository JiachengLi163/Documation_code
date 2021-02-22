#!/bin/bash

if [ ! -d "out" ]; then
  mkdir out
fi

GCCVERSION=gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu

export OUT=$PWD/out
export PATH=$PATH:$PWD/toolchain/$GCCVERSION/bin
export CROSS_COMPILE=aarch64-linux-gnu-
# atf
export MV_DDR_PATH=$PWD/uboot-marvell/mv-ddr-marvell
export BL33=$OUT/u-boot.bin
export SCP_BL2=$PWD/uboot-marvell/binaries-marvell/mrvl_scp_bl2*.img

