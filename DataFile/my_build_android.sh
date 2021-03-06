#!/bin/bash
export TMPDIR=/home/xhc/tmpdir
NDK=/home/xhc/ffmpeg/android-ndk-r15c
SYSROOT=$NDK/platforms/android-16/arch-arm/
TOOLCHAIN=/home/xhc/ffmpeg/android-ndk-r15c/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64
CPU=arm
PREFIX=/home/xhc/result2/$CPU
ADDI_CFLAGS="-marm"
function build_one
{
./configure \
--prefix=$PREFIX \
--enable-libfreetype \
--enable-shared \
--disable-static \
--disable-doc \
--disable-ffmpeg \
--disable-ffplay \
--disable-ffprobe \
--disable-ffserver \
--disable-doc \
--disable-stripping \
--enable-small \
--enable-encoder=libfdk_aac \
--enable-decoder=libfdk_aac \
--enable-libfdk-aac \
--cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
--target-os=android \
--arch=arm \
--enable-cross-compile \
--sysroot=$SYSROOT \
--extra-cflags="-Os -fpic $ADDI_CFLAGS" \
--extra-ldflags="$ADDI_LDFLAGS" \
$ADDITIONAL_CONFIGURE_FLAG
make clean
make
make install
}
build_one
