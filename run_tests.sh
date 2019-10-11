#!/bin/bash

export CLANG_BINARY="$(which clang)"

llvm-lit -sv ./
