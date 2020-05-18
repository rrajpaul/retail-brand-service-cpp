#!/bin/sh

cd /app

rm -rf tmp

mkdir tmp

cd tmp

##########################################################
## install mongo-c-driver-1.13.1

wget https://github.com/mongodb/mongo-c-driver/releases/download/1.13.1/mongo-c-driver-1.13.1.tar.gz

tar -xzvf mongo-c-driver-1.13.1.tar.gz

cd mongo-c-driver-1.13.1/build

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=/usr/local ..

make 

make install

##########################################################

cd ../../..

rm -rf tmp