#!/bin/bash

cd ./build

make clean

cmake ..

make

../bin/MacGame