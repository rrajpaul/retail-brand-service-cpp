docker exec -i retail_brand_data1 sh -c 'mongoimport -d retail-brand -c style --type csv --headerline --drop ' < style.csv
docker exec -i retail_brand_data1 sh -c 'mongoimport -d retail-brand -c sku --type csv --headerline --drop ' < sku.csv
