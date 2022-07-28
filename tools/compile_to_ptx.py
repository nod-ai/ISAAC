#!/usr/bin/env python3

# This file compiles a .cu file to ptx using LLVM
# Assumes CUDA SDK has been installed into /usr/local/cuda

import argparse
import subprocess
import os

parser = argparse.ArgumentParser(description='Compile .cu file to ptx')
parser.add_argument('-input', type=str, help='the input .cu file', required=True)
parser.add_argument('-clang-path', type=str, help='path to clang', required=True)
parser.add_argument('-output', type=str, help='name of the output .ptx file', default='output.ptx')
parser.add_argument('-emit-ll', default=False, action='store_true', help='whether to output a ll file or not')
parser.add_argument('-arch', type=str, help='cuda gpu arch to target', default='sm_80')
args = parser.parse_args()

if args.output[-4:] != '.ptx':
    args.output += '.ptx'

cmd = [f'{args.clang_path}', f'{args.input}', '-S', f'--cuda-gpu-arch={args.arch}']
print(' '.join(cmd))
subprocess.run(cmd)

# Remove host asm and rename gpu asm to ptx
prefix = os.path.basename(args.input[0:-3])
subprocess.run(['rm', f'{prefix}.s'])
subprocess.run(['mv', f'{prefix}-cuda-nvptx64-nvidia-cuda-{args.arch}.s', f'{args.output}'])

# Check if ll file generation was requested
if args.emit_ll:
    cmd += ['-emit-llvm']
    print(' '.join(cmd))
    subprocess.run(cmd)
    subprocess.run(['rm', f'{prefix}.ll'])
    subprocess.run(['mv', f'{prefix}-cuda-nvptx64-nvidia-cuda-{args.arch}.ll', f'{prefix}.ll'])
