#!/usr/bin/env bash

cd /opt/

git clone --recurse-submodules https://github.com/fyrware/grok.git
cd grok/

sh scripts/build_libgit2.sh

touch CMakeLists.txt
echo "cmake_minimum_required(VERSION 3.10)" >> CMakeLists.txt
echo "include(.grokmake)" >> CMakeLists.txt

mkdir binary/
cd binary/

cmake ..
cmake --build . --target all --clean-first

echo -e "\n" >> ~/.bashrc
echo "PATH=\"\$PATH:/opt/grok/binary\"" >> ~/.bashrc
source ~/.bashrc
