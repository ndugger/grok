#!/bin/bash

cd libraries/git2

mkdir build/
cd build/

cmake ..
cmake --build .