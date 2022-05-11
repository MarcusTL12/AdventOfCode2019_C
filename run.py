#!/usr/bin/env python3

import subprocess
import sys

from time import time

ind = 1

if len(sys.argv) >= 2 and sys.argv[1] == "c":
    subprocess.call("./compile.py")
    ind += 1

excecutable_name = "./build/aoc/aoc"

run_command = [f"{excecutable_name}"]

for i in range(ind, len(sys.argv)):
    run_command.append(sys.argv[i])

s = f"Running {excecutable_name}:"
print(s)
print("=" * len(s) * 2)

t = time()
subprocess.call(' '.join(run_command), shell=True)
t = time() - t
print("=" * len(s) * 2)
print(f"Run time: {t:.4f} s")
