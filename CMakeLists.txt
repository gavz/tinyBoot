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

############################## CONFIGURATION ###################################

# CMake System Configuration
set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)
set(CMAKE_BUILD_TYPE                Debug)
set(CMAKE_TRY_COMPILE_TARGET_TYPE   STATIC_LIBRARY)

########################## Compiler configuration ##############################

# Project Setup
project("tinyOS" VERSION 0.0.1 LANGUAGES C ASM)

# Compiler Setup
set(TOOLCHAIN "/Users/h3/Work/toolchains/arm-gnu-toolchain-12.2.rel1-darwin-arm64-aarch64-none-elf/bin")
set(CMAKE_C_COMPILER    ${TOOLCHAIN}/aarch64-none-elf-gcc)
set(CMAKE_ASM_COMPILER  ${TOOLCHAIN}/aarch64-none-elf-gcc)
set(CMAKE_LINKER        ${TOOLCHAIN}/aarch64-none-elf-ld)
set(CMAKE_OBJCOPY       ${TOOLCHAIN}/aarch64-none-elf-objcopy)

set(CMAKE_SKIP_SIMPLE_TESTS     TRUE)
set(CMAKE_C_COMPILER_WORKS      1)
set(CMAKE_LINKER_WORKS          1)

set(TBOOT_LINKER_SCRIPT     ${CMAKE_SOURCE_DIR}/target/tboot/linker.lds)
set(TINYROM_LINKER_SCRIPT   ${CMAKE_SOURCE_DIR}/target/tinyrom/linker.lds)

add_definitions (-DPLATFORM_NAME="aarch64-virt")
add_definitions (-DPLATFORM_ID="p10000")

# Version Generator
include(conf/version.cmake)

########################### Target configuration ###############################

# The first thing to run on the device is tinyROM, thus, it's built first.
add_executable(tinyROM)
target_include_directories(tinyROM PRIVATE include .)
target_compile_options(tinyROM PRIVATE
    -mgeneral-regs-only
    -march=armv8-a
    -pie
    -O1
    -ffreestanding
    -nostdlib
    -nostartfiles
)
target_link_options(tinyROM PRIVATE
    -nodefaultlibs
    -nostartfiles
    -Iinclude
    -T ${TINYROM_LINKER_SCRIPT}
)

# tBoot is loaded by tinyROM, so that's built second. The only difference here
# is that tBoot is built as an ELF, and therefore requires a linker script.
add_executable(tBoot)
target_include_directories(tBoot PRIVATE include .)
target_compile_options(tBoot PRIVATE
    -mgeneral-regs-only
    -march=armv8-a
    -O1
    -ffreestanding
    -nostdlib
    -nostartfiles
)
target_link_options(tBoot PRIVATE
    -nodefaultlibs
    -nostartfiles
    -Iinclude
    -T ${TBOOT_LINKER_SCRIPT}
)

target_sources(tinyROM PRIVATE target/tinyrom/main.c)
target_sources(tBoot PRIVATE
    target/tboot/main.c
    target/tboot/console.c
    target/tboot/command.c

    target/tboot/commands/exception.c
    target/tboot/commands/version.c
    target/tboot/commands/bootm.c
    target/tboot/commands/bdev.c
    target/tboot/commands/mem.c

#    common/main.c
#    common/console.c
#    common/command.c
#
#    common/commands/exception.c
#    common/commands/version.c
#    common/commands/drvinfo.c
#    common/commands/bootm.c
#    common/commands/bdev.c
#    common/commands/mem.c
)

# Include the application-specific sources
#add_subdirectory(target/tinyrom)
#add_subdirectory(target/tboot)

# Include sources for various components/modules
add_subdirectory(arch/)
add_subdirectory(lib/)
add_subdirectory(sys/)
add_subdirectory(drivers/)

# Include platform sources
add_subdirectory(plat/${TARGET_PLATFORM})

# Generate version
add_dependencies(tinyROM generate_version)
add_dependencies(tBoot generate_version)

add_custom_command(TARGET tinyROM POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_BINARY_DIR}/tinyROM 
  									 ${CMAKE_BINARY_DIR}/tinyROM.img)

add_custom_command(TARGET tBoot POST_BUILD
COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_BINARY_DIR}/tBoot 
                                    ${CMAKE_BINARY_DIR}/tBoot.img)
