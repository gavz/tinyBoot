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

#ifndef __SYS_SYMBOLS_H__
#define __SYS_SYMBOLS_H__

/* header symbols */
#define __BEGIN_DECLS
#define __END_DECLS

/* assembler symbols */
#define L(l)            .L ## l
#define LOCAL(sym)      L_ ## sym

/* marking exported symbols */
#define EXPORT_FUNC(x)      x
#define LIBC_FUNC(x)        x
#define EXPORT_VAR(x)       x
#define STATIC_VAR(x)       x

#endif /* __sys_symobls_h__ */