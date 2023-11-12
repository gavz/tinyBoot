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

#ifndef __LIB_IMAGE_H__
#define __LIB_IMAGE_H__

#include <sys/types.h>

struct timg_header
{
    uint32_t        magic;
    uint32_t        version;
    uint32_t        size;
    uint32_t        flags;
    uint32_t        type;
};

/* Image magics */
#define TIMG_HEADER_MAGIC       0xfeedc0de
#define TIMB_HEADER_CIGAM       0xdec0edfe

/* Image versions */
#define TIMG_VERSION_1_0        0x1

/* Image types */
#define TIMG_IMAGE_TYPE_TBOT    "tbot"      // tBoot Stage 2
#define TIMG_IMAGE_TYPE_TMON    "tmon"      // tBoot Secure Monitor
#define TIMG_IMAGE_TYPE_KERN    "kern"      // tinyKern
#define TIMG_IMAGE_TYPE_LINX    "linx"      // Linux Kernel

/* Image flags */
// ...





#endif /* __lib_image_h__ */

