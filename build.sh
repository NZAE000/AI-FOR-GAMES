#! bin/bash

app=$1

make clean && make CC='ccache clang++' -j && ./$app
