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

cmake_minimum_required(VERSION 3.10)

set(VERSION_CMD "${CMAKE_CURRENT_SOURCE_DIR}/conf/version.py")
set(MASTER_VERSION "${CMAKE_CURRENT_SOURCE_DIR}/common/MasterVersion")
set(TEMPLATE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/conf/")
set(OUTFILE "${CMAKE_CURRENT_SOURCE_DIR}/include/version.h")

set(BUILD_TYPE "${CMAKE_BUILD_TYPE}")
set(PLATFORM "aarch64-virt")

add_custom_target(generate_version
    DEPENDS ${VERSION_CMD}
            ${CMAKE_CURRENT_SOURCE_DIR}/*
)

add_custom_command(TARGET generate_version
    PRE_BUILD
        COMMAND python3 ${VERSION_CMD} -m ${MASTER_VERSION} -t ${TEMPLATE_DIR} 
                        -o ${OUTFILE} -b ${BUILD_TYPE} -p ${PLATFORM}
)
