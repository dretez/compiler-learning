#!/bin/bash

clear
make
cat tests/test.txt
./build/main tests/test.txt
