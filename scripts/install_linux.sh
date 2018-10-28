#!/usr/bin/env bash

#### install grok in top level /opt/ directory
cd /opt/

#### download grok source files
git clone --recurse-submodules https://github.com/fyrware/grok.git
cd grok/
chmod 777 . -R
git fetch

#### build libgit2
cd libraries/git2/

mkdir build/
cd build/

cmake ..
cmake --build .

cd ../../../

#### create CMakeLists.txt file
touch CMakeLists.txt
echo "cmake_minimum_required(VERSION 3.10)" >> CMakeLists.txt
echo "include(.grokmake)" >> CMakeLists.txt

#### create output binary folder
mkdir binary/
cd binary/

#### build grok
cmake ..
cmake --build . --target all --clean-first

#### add grok to PATH
echo -e "\n" >> ~/.bashrc
echo "PATH=\"\$PATH:/opt/grok/binary\"" >> ~/.bashrc
source ~/.bashrc
