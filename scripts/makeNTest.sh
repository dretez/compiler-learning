#!/bin/bash

clear
make
cat tests/test.txt
./build/compiler tests/test.txt
