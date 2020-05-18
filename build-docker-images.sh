#!/bin/sh

# This step builds the mongodb image
cd utility

docker build -t retail-brand-mongo-data:1.0 .

cd ..

# This step builds the dependent libraries - mongo-c-driver, mongo-cxx-driver, oatpp, oatpp-swagger, oatpp-mongocxxmapper
docker build -t retail-brand-mongo-dependency:1.0 -f Dockerfile.dependency .
docker build -t retail-brand-mongo-service:1.0  -f Dockerfile.webapp .

# Clean up docker images
docker images purge

# This step runs docker containers for mongodb and the web app
docker run -d --net bridge -p 27017 --name retail_brand_data1 retail-brand-mongo-data:1.0 
docker run -d --net bridge -p 9000 --name retail_brand_service1 retail-brand-mongo-service:1.0 -e `docker inspect retail_brand_data1 --format '{{ .NetworkSettings.IPAddress }}'`

cd utility/sample_data

# This step adds data to mongodb
./install-sample-data.sh
