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

from jinja2 import Environment, FileSystemLoader
from dataclasses import dataclass
import jinja2 as j2
import argparse

VERBOSE = True

@dataclass
class Version:
    data: list
    build: int
    s_maj: int
    s_min: int
    s_rev: int

def log(msg):
    if VERBOSE: print("[log]: {}".format(msg))

def parse_master_version_file(file, buildtype, plat):
    log("parsing file: {}".format(file))

    with open(file, "r") as f:
        data = f.readlines()
    
    build_version = data[0].strip().split(".")

    # these need to be ints
    source_major = int(data[1].strip().split(".")[0])
    source_minor = int(data[2].strip().split(".")[0])
    source_rev = int(data[3].strip().split(".")[0])

    build_version = "{}{}{}".format(build_version[0], build_version[1], build_version[2])
    source_version = "tboot-{}.{}.{}.{}".format(build_version, source_major, source_minor, source_rev)

    version = Version(
        data=data, build=build_version, s_maj=source_major, s_min=source_minor, s_rev=source_rev
    )
    return version

def update_source_number(version):
    if version.s_min >= 99:
        version.s_maj += 1
        version.s_min = 0
    else:
        version.s_min += 1
    SOURCE_VERSION = f"tboot-{version.build}.{version.s_maj}.{version.s_min}.{version.s_rev}"
    print("New source version: {}".format(SOURCE_VERSION))
    return version

def write_master_version_file(file, version):
    version.data[1] = f"{version.s_maj}\n"
    version.data[2] = f"{version.s_min}\n"
    with open(file, "w") as f:
        f.writelines(version.data)

def write_version_header(fn, template, version, type, plat):
    env = j2.Environment(loader=FileSystemLoader(template))
    template = env.get_template("version.h.j2")

    source_version = "tboot-{}.{}.{}.{}".format(version.build, version.s_maj, version.s_min, version.s_rev)
    header = template.render(
        TBOOT_BUILD_VERSION_VALUE = source_version,
        TBOOT_BUILD_STYLE_VALUE = type,
        TBOOT_BUILD_PLAT_TARGET = plat
    )
    with open(fn, 'w') as f:
        f.write(header)
    print(f"path: {fn}:\n{header}")

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("-m", "--master", action="store")
    parser.add_argument("-t", "--template", action="store")
    parser.add_argument("-o", "--outfile", action="store")
    parser.add_argument("-b", "--build-type", action="store")
    parser.add_argument("-p", "--platform", action="store")
    args = parser.parse_args()

    if args.master:
        parsed_version = parse_master_version_file(args.master, args.build_type, args.platform)
        new_version = update_source_number(parsed_version)

        write_master_version_file(args.master, new_version)
        write_version_header(args.outfile, args.template, new_version, args.build_type, args.platform)