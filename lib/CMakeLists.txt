#===----------------------------------------------------------------------===//
#
#                                  tinyOS
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#	Copyright (C) 2023, Is This On? Holdings Ltd
#
#  Harry Moulton <me@h3adsh0tzz.com>
#
#===----------------------------------------------------------------------===//

cmake_minimum_required(VERSION 3.13)

########################## Compiler configuration ##############################

# Sources
list(APPEND lib_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/heap.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ctype.c
    ${CMAKE_CURRENT_SOURCE_DIR}/display.c
    ${CMAKE_CURRENT_SOURCE_DIR}/serial.c
    ${CMAKE_CURRENT_SOURCE_DIR}/blockdev.c

    ${CMAKE_CURRENT_SOURCE_DIR}/libc/printf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libc/tests.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libc/memchr.c  
    ${CMAKE_CURRENT_SOURCE_DIR}/libc/memcpy.c  
    ${CMAKE_CURRENT_SOURCE_DIR}/libc/memset.c  
    ${CMAKE_CURRENT_SOURCE_DIR}/libc/memmove.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/libc/memcmp.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/libc/strchr.c  
    ${CMAKE_CURRENT_SOURCE_DIR}/libc/strlcpy.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/libc/strncmp.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/libc/strrchr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libc/strcmp.c  
    ${CMAKE_CURRENT_SOURCE_DIR}/libc/strlen.c  
    ${CMAKE_CURRENT_SOURCE_DIR}/libc/strnlen.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/libc/strtoul.c
)

target_sources(tinyROM PRIVATE ${lib_sources})
target_sources(tBoot PRIVATE ${lib_sources})
