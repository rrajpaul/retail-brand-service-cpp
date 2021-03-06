#ifndef Database_hpp
#define Database_hpp

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <boost/optional.hpp>
#include <string>
#include "oatpp/core/Types.hpp"
#include "oatpp-mongocxxmapper/ObjectMapper.hpp"
#include "dto/DTOs.hpp"
#include "MongoUrl.hpp"
#include <iostream>
#include <cstdlib>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

const std::string TAG = "retail-brand-service-cpp ";

const std::string DB_HOST = "localhost:27017";
const std::string DB_USER = "userAdmin";
const std::string DB_PASSWORD = "Khill-59!";
const std::string DB_NAME = "retail-brand";
const std::string COLLECTION_STYLE = "style";
const std::string COLLECTION_SKU = "sku";
//static std::string MONGO_URL;

class Database : public oatpp::base::Countable {
private:
    std::shared_ptr<oatpp::parser::mongocxx::mapping::ObjectMapper> m_mongomapper;
    mongocxx::client m_client_style;
    mongocxx::client m_client_sku;
    mongocxx::database m_db_style;
    mongocxx::database m_db_sku;
	mongocxx::collection m_coll_style;
    mongocxx::collection m_coll_sku;
    v_int32 getStyleMaxId();
    v_int32 getSkuMaxId();
    
public:
    Database() {
        std::string connString;

        MongoUrl url;

        std::string mongo_url = url.getMongUrl();

        if(mongo_url == "127.0.0.1" || mongo_url == "127.0.1.1") {
            mongo_url = "localhost:27017";
        }
        
        OATPP_LOGD("Database", "Database mongoUrl %s...",mongo_url.c_str());
        
        connString =  "mongodb://" + mongo_url;

        std::cout << mongo_url << std::endl;

        m_client_style = mongocxx::client{mongocxx::uri{connString}};
        m_client_sku= mongocxx::client{mongocxx::uri{connString}};
        m_db_style = m_client_style[DB_NAME];
        m_db_sku = m_client_sku[DB_NAME];
        m_coll_style = m_db_style[COLLECTION_STYLE];
        m_coll_sku = m_db_sku[COLLECTION_SKU];
        m_mongomapper =  oatpp::parser::mongocxx::mapping::ObjectMapper::createShared();
        
    }
    
	StyleDto::ObjectWrapper createStyle(const StyleDto::ObjectWrapper& style);
	StyleDto::ObjectWrapper updateStyle(const StyleDto::ObjectWrapper& style);
	StyleDto::ObjectWrapper getStyleById(v_int32 id);
    oatpp::data::mapping::type::List<SkuDto::ObjectWrapper>::ObjectWrapper getSkuByStyleId(v_int32 id);
	oatpp::data::mapping::type::List<StyleDto::ObjectWrapper>::ObjectWrapper getStyles(v_int32 pageNumber, v_int32 nPerPage);
    oatpp::data::mapping::type::Int32 getTotalStylesCount();
	bool deleteStyle(v_int32 id);
    bool deleteSkuByStyleId(v_int32 id);

    SkuDto::ObjectWrapper createSku(const SkuDto::ObjectWrapper& sku);
	SkuDto::ObjectWrapper updateSku(const SkuDto::ObjectWrapper& sku);
	SkuDto::ObjectWrapper getSkuById(v_int32 id);
	oatpp::data::mapping::type::List<SkuDto::ObjectWrapper>::ObjectWrapper getSkus();
    bool deleteSku(v_int32 id);    
};

#endif