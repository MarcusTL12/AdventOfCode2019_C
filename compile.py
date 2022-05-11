#!/usr/bin/env python3

import subprocess
import sys
import os

from time import time

c_compiler = "clang-12"

buildsystem = "Ninja"
# buildsystem = '"Unix Makefiles"'

if len(sys.argv) >= 2 and sys.argv[1] == "debug":
    cfg = "Debug"
else:
    cfg = "Release"

if not os.path.exists("build"):
    os.mkdir("build")

os.chdir("build")

# Compile util

if not os.path.exists("aoc_util"):
    os.mkdir("aoc_util")

os.chdir("aoc_util")

aoc_util_command = ["cmake"]
aoc_util_command.append("../../AoC_C_utils/")
aoc_util_command.append(f"-DCMAKE_INSTALL_PREFIX={cfg}")
aoc_util_command.append(f"-DCMAKE_C_COMPILER={c_compiler}")
aoc_util_command.append(f"-DCMAKE_BUILD_TYPE={cfg}")
aoc_util_command.append(f"-G{buildsystem}")

subprocess.call(' '.join(aoc_util_command), shell=True)
subprocess.call("cmake --build . --target install", shell=True)

# Compile year

os.chdir("..")

if not os.path.exists("aoc"):
    os.mkdir("aoc")

os.chdir("aoc")

aoc_command = ["cmake"]
aoc_command.append("../../")
aoc_command.append(f"-DAOC_UTIL_INSTALL=build/aoc_util/{cfg}")
aoc_command.append(f"-DCMAKE_C_COMPILER={c_compiler}")
aoc_command.append(f"-DCMAKE_BUILD_TYPE={cfg}")
aoc_command.append(f"-G{buildsystem}")

subprocess.call(' '.join(aoc_command), shell=True)
subprocess.call("cmake --build .", shell=True)
