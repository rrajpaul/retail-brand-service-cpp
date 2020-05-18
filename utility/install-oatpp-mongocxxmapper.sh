#!/bin/sh

cd /app

rm -rf tmp

mkdir tmp

cd tmp

##########################################################
## install oatpp-mongocxxmapper

MODULE_NAME="oatpp-mongocxxmapper"

git clone https://github.com/rrajpaul/$MODULE_NAME.git

cd $MODULE_NAME

mkdir build

cd build

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=/usr/local ..

make 

make install

##########################################################

cd ../../..

rm -rf tmp
