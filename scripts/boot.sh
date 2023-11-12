##===----------------------------------------------------------------------===//
##
##                                  tinyOS
##
##  This program is free software: you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation, either version 3 of the License, or
##  (at your option) any later version.
##
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with this program.  If not, see <http://www.gnu.org/licenses/>.
##
##	Copyright (C) 2023, Is This On? Holdings Ltd
##
##  Harry Moulton <me@h3adsh0tzz.com>
##
##===----------------------------------------------------------------------===//

# Default load addresses for tBoot and Kernel
TBOOT_LOAD_ADDR=0x48000000
KERNEL_LOAD_ADDR=0x48500000

FIRMWARE0_ADDR=${TBOOT_LOAD_ADDR}
FIRMWARE1_ADDR=${KERNEL_LOAD_ADDR}

qemu-system-aarch64 \
    -M virt -cpu cortex-a76 -m 512M --machine secure=true,virtualization=on,gic-version=3 \
    -bios ${TINYROM_RAW} \
    -device loader,file=${FIRMWARE0},addr=${FIRMWARE0_ADDR} \
    -device loader,file=${FIRMWARE1},addr=${FIRMWARE1_ADDR} \
    -serial stdio  \
    -d int -s