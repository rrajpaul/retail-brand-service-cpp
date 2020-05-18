#!/bin/sh

cd /app

rm -rf tmp

mkdir tmp

cd tmp

##########################################################
## install oatpp-swagger

MODULE_NAME="oatpp-swagger"

wget https://github.com/oatpp/oatpp-swagger/archive/1.0.0.tar.gz

tar -xzvf 1.0.0.tar.gz

cd "$MODULE_NAME-1.0.0"

mkdir build

cd build

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=/usr/local ..

make 

make install

##########################################################

cd ../../..

rm -rf tmp
