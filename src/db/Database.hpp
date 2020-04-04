#ifndef Database_hpp
#define Database_hpp

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <boost/optional.hpp>
#include <string>
#include "oatpp/core/Types.hpp"
#include "oatpp-mongocxxmapper/ObjectMapper.hpp"
#include "dto/DTOs.hpp"
#include <iostream>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;

const std::string TAG = "Mongodb example ";
const std::string DB_HOST = "localhost:27017:9000";
const std::string DB_USER = "userAdmin";
const std::string DB_PASSWORD = "Khill-59!";
const std::string DB_NAME = "retail-brand";
const std::string COLLECTION_STYLE = "style";
const std::string COLLECTION_SKU = "sku";

class Database : public oatpp::base::Countable {
private:
    std::shared_ptr<oatpp::parser::mongocxx::mapping::ObjectMapper> m_mongomapper;
    mongocxx::client m_client;
    mongocxx::database m_db;
	mongocxx::collection m_coll;
    mongocxx::collection m_coll_sku;
    v_int32 getMaxId();
public:
    Database() {
        std::string connString;
        //connString =  "mongodb://" + DB_USER + ":" + DB_PASSWORD + "@" + DB_HOST;
        connString =  "mongodb://" + DB_HOST;
        m_client = mongocxx::client{mongocxx::uri{connString}};
        m_db = m_client[DB_NAME];
        m_coll = m_db[COLLECTION_STYLE];
        m_coll_sku = m_db[COLLECTION_SKU];
        m_mongomapper =  oatpp::parser::mongocxx::mapping::ObjectMapper::createShared();
    }
    
	StyleDto::ObjectWrapper createStyle(const StyleDto::ObjectWrapper& style);
	StyleDto::ObjectWrapper updateStyle(const StyleDto::ObjectWrapper& style);
	StyleDto::ObjectWrapper getStyleById(v_int32 id);
    oatpp::data::mapping::type::List<SkuDto::ObjectWrapper>::ObjectWrapper getSkuByStyleId(v_int32 id);
	oatpp::data::mapping::type::List<StyleDto::ObjectWrapper>::ObjectWrapper getStyles();
	bool deleteStyle(v_int32 id);

};

#endif