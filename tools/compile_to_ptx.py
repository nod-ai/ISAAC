#!/usr/bin/env python3

# This file compiles a .cu file to ptx using LLVM
# Assumes CUDA SDK has been installed into /usr/local/cuda

import argparse
import subprocess
import os

parser = argparse.ArgumentParser(description='Compile .cu file to ptx')
parser.add_argument('-i', type=str, help='the input .cu file', required=True)
parser.add_argument('-c', type=str, help='path to clang', required=True)
parser.add_argument('-o', type=str, help='name of the output .ptx file', default='output.ptx')
parser.add_argument('-a', type=str, help='cuda gpu arch to target', default='sm_80')
args = parser.parse_args()

if args.o[-4:] != '.ptx':
    args.o += '.ptx'

cmd = [f'{args.c}', f'{args.i}', '-S', f'--cuda-gpu-arch={args.a}']
print(' '.join(cmd))
subprocess.run(cmd, check=True)

# Remove host asm and rename gpu asm to ptx
prefix = os.path.basename(args.i[0:-3])
subprocess.run(['rm', f'{prefix}.s'])
subprocess.run(['mv', f'{prefix}-cuda-nvptx64-nvidia-cuda-{args.a}.s', f'{args.o}'])
