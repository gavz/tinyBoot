//===----------------------------------------------------------------------===//
//
//                                  tinyOS
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//	Copyright (C) 2023, Is This On? Holdings Ltd
//
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

#ifndef __SYS_BOOT_H__
#define __SYS_BOOT_H__

typedef uint64_t    jump_addr_t;

/* supported boot devices */
enum boot_device 
{
    BOOT_DEVICE_UNKNWON,
    BOOT_DEVICE_VIRTIO,
    BOOT_DEVICE_PFLASH,
    BOOT_DEVICE_MEMBLK,
    BOOT_DEVICE_NVME,
};

void prepare_and_jump (jump_addr_t *ptr);

#endif /* __sys_root_h__ */
