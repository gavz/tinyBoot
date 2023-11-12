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
list(APPEND arch_sources
    # Assembly
    ${CMAKE_CURRENT_SOURCE_DIR}/start.S
    ${CMAKE_CURRENT_SOURCE_DIR}/trampoline.S
    ${CMAKE_CURRENT_SOURCE_DIR}/asm.S
    ${CMAKE_CURRENT_SOURCE_DIR}/memzero.S
    ${CMAKE_CURRENT_SOURCE_DIR}/exception.S
    
    # C
    ${CMAKE_CURRENT_SOURCE_DIR}/handlers.c
    ${CMAKE_CURRENT_SOURCE_DIR}/timer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu.c
)

target_sources(tinyROM PRIVATE ${arch_sources})
target_sources(tBoot PRIVATE ${arch_sources})

target_include_directories(tinyROM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(tBoot PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
