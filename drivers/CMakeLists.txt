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
list(APPEND driver_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/serial/serial-pl011.c
    ${CMAKE_CURRENT_SOURCE_DIR}/blockdev/mem-blk.c
)

target_sources(tinyROM PRIVATE ${driver_sources})
target_sources(tBoot PRIVATE ${driver_sources})
