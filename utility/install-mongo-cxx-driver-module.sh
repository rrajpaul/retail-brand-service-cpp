#!/bin/sh

cd /app

rm -rf tmp

mkdir tmp

cd tmp

##########################################################
## install mongo-cxx-driver-r3.4.0

wget https://github.com/mongodb/mongo-cxx-driver/archive/r3.4.0.tar.gz

tar -xzvf r3.4.0.tar.gz

cd mongo-cxx-driver-r3.4.0/build

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=/usr/local ..

make 

make install

##########################################################

cd ../../..

rm -rf tmp
