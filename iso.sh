#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/bootstrapped.kernel isodir/boot/bootstrapped.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "bootstrapped" {
	multiboot /boot/bootstrapped.kernel
}
EOF
grub-mkrescue -o bootstrapped.iso isodir
