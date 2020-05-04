#include "Database.hpp"

v_int32 Database::getStyleMaxId(){
  bsoncxx::builder::stream::document sort_doc{};
  sort_doc << "StyleId" << -1 ;
  mongocxx::options::find opts;
  opts.sort(sort_doc.view()).limit(1);

  auto cursor = m_coll_style.find({}, opts);
  auto style = m_mongomapper->readFromDocument<StyleDto::ObjectWrapper>(*cursor.begin());
  auto maxId = style->StyleId + 10;

  return maxId;
}

StyleDto::ObjectWrapper Database::createStyle(const StyleDto::ObjectWrapper& style){
    auto newId = getStyleMaxId();
    style->StyleId = newId;
    
    auto mongoStyle = m_mongomapper->writeAsDocument(style);
    auto result = m_coll_style.insert_one(mongoStyle.view());
    auto resultStyle = StyleDto::ObjectWrapper();

    if(result) {
        OATPP_LOGD(TAG, "Inserted new style '%s' (%llu) with id %s", 
                        style->Description->c_str(), style->StyleId->getValue(), 
                        result->inserted_id().get_oid().value.to_string().c_str());

        auto insertedStyle = getStyleById(style->StyleId);                            
        return  resultStyle = style;
    }

    return resultStyle;
}

StyleDto::ObjectWrapper Database::updateStyle(const StyleDto::ObjectWrapper& style){
    auto foundStyle = getStyleById(style->StyleId);
    auto resultStyle = StyleDto::ObjectWrapper();
                
    if(foundStyle->StyleId->getValue() == style->StyleId->getValue()) {    
        auto mongoStyle = m_mongomapper->writeAsDocument(style);
        auto mongoFound = m_mongomapper->writeAsDocument(foundStyle);
        auto result =   m_coll_style.replace_one(
                        mongoFound.view(),
                        mongoStyle.view()
        );

        if(result->modified_count() > 0) {
                OATPP_LOGD(TAG, "Update style '%s' with StyleId: (%llu) ", style->Description->c_str(), style->StyleId->getValue());
                resultStyle = style;
        }
    }

    return resultStyle;
}

StyleDto::ObjectWrapper Database::getStyleById(v_int32 id){
    auto style = StyleDto::ObjectWrapper();    

    auto result = m_coll_style.find_one({document{} << "StyleId" << id << finalize});
    if(result) {
        auto docView = result->view();
        style = m_mongomapper->readFromDocument<StyleDto::ObjectWrapper>(docView);

        OATPP_LOGD(TAG, "Found style with id %d", style->StyleId->getValue()); 
    }

    return style;
}

oatpp::data::mapping::type::List<SkuDto::ObjectWrapper>::ObjectWrapper Database::getSkuByStyleId(v_int32 id){   
    auto result = m_coll_style.find_one({document{} << "StyleId" << id << finalize});
    auto listDocs = oatpp::data::mapping::type::List<SkuDto::ObjectWrapper>::createShared();

    if(result) {
        OATPP_LOGD(TAG, "Found Style with id %d", id); 
        mongocxx::options::find opts;
        opts.sort(make_document(kvp("StyleId", 1), kvp("Size", 1), kvp("Color", 1)));

        auto cursor_sku = m_coll_sku.find(document{} << "StyleId" << id << finalize, opts);
        
        for(auto doc : cursor_sku) {        
            listDocs->pushBack(m_mongomapper->readFromDocument<SkuDto::ObjectWrapper>(doc));
            std::cout << TAG << "Found Sku with SkuNumber " << m_mongomapper->readFromDocument<SkuDto::ObjectWrapper>(doc)->SkuNumber << std::endl;
        }   
    }

    return listDocs;
}

oatpp::data::mapping::type::List<StyleDto::ObjectWrapper>::ObjectWrapper Database::getStyles(v_int32 pageNumber, v_int32 nPerPage){
    mongocxx::options::find opts;
    opts.skip( pageNumber > 0 ? ( ( pageNumber - 1 ) * nPerPage ) : 0 ).limit( nPerPage ).sort(make_document(kvp("StyleId", 1), kvp("Manufacturer", 1), kvp("Brand", 1)));
    auto cursor = m_coll_style.find({}, opts);
    auto listDocs = oatpp::data::mapping::type::List<StyleDto::ObjectWrapper>::createShared();

    for(auto doc : cursor) {        
        listDocs->pushBack(m_mongomapper->readFromDocument<StyleDto::ObjectWrapper>(doc));
    }

    return listDocs;
}

oatpp::data::mapping::type::Int32 Database::getTotalStylesCount() {  
    auto cursor = m_coll_style.find({});
    auto listDocs = oatpp::data::mapping::type::List<StyleDto::ObjectWrapper>::createShared();

    for(auto doc : cursor) {        
        listDocs->pushBack(m_mongomapper->readFromDocument<StyleDto::ObjectWrapper>(doc));
    }

    return listDocs->count();
}

bool Database::deleteStyle(v_int32 id){
    auto result = m_coll_style.delete_one(document{} << "StyleId" << id << finalize);

    if(result) {
        OATPP_LOGD(TAG, "Deleted style document with id %d", id); 
        return true;
    }

    return false;
}

bool Database::deleteSkuByStyleId(v_int32 id){   
    auto result = m_coll_sku.delete_many(document{} << "StyleId" << open_document <<
            "$eq" << id << close_document << finalize);
    if(result) {
        OATPP_LOGD(TAG, "Deleted %d skus with id %d", id); 

        return true; 
    }

    return false;
}

v_int32 Database::getSkuMaxId(){
  bsoncxx::builder::stream::document sort_doc{};
  sort_doc << "SkuNumber" << -1 ;
  mongocxx::options::find opts;
  opts.sort(sort_doc.view()).limit(1);

  auto cursor = m_coll_style.find({}, opts);
  auto sku = m_mongomapper->readFromDocument<SkuDto::ObjectWrapper>(*cursor.begin());
  auto maxId = sku->SkuNumber + 10;

  return maxId;
}

SkuDto::ObjectWrapper Database::createSku(const SkuDto::ObjectWrapper& sku){
    auto newId = getSkuMaxId();
    sku->SkuNumber = newId;
    
    auto mongoSku = m_mongomapper->writeAsDocument(sku);
    auto result = m_coll_sku.insert_one(mongoSku.view());
    auto resultSku = SkuDto::ObjectWrapper();

    if(result) {
        OATPP_LOGD(TAG, "Inserted new styleId '%d' (%llu) with id %s", 
                        sku->SkuNumber, sku->SkuNumber->getValue(), 
                        result->inserted_id().get_oid().value.to_string().c_str());

        auto insertedSku = getSkuById(sku->SkuNumber);                            
        return  resultSku = sku;
    }

    return resultSku;
}

SkuDto::ObjectWrapper Database::updateSku(const SkuDto::ObjectWrapper& sku){
    auto foundSku = getSkuById(sku->SkuNumber);
    auto resultSku = SkuDto::ObjectWrapper();
                
    if(foundSku->StyleId->getValue() == sku->StyleId->getValue()) {    
        auto mongoStyle = m_mongomapper->writeAsDocument(sku);
        auto mongoFound = m_mongomapper->writeAsDocument(foundSku);
        auto result =   m_coll_sku.replace_one(
                        mongoFound.view(),
                        mongoStyle.view()
        );

        if(result->modified_count() > 0) {
                OATPP_LOGD(TAG, "Updated sku '%d' with StyleId: (%llu) ", sku->StyleId, sku->SkuNumber->getValue());
                resultSku = sku;
        }
    }

    return resultSku;
}

SkuDto::ObjectWrapper Database::getSkuById(v_int32 id){
    auto sku = SkuDto::ObjectWrapper();    
    auto result = m_coll_sku.find_one({document{} << "StyleId" << id << finalize});
    if(result) {
        auto docView = result->view();
        sku = m_mongomapper->readFromDocument<SkuDto::ObjectWrapper>(docView);

        OATPP_LOGD(TAG, "Found style with id %d", sku->SkuNumber->getValue()); 
    }

    return sku;
}

oatpp::data::mapping::type::List<SkuDto::ObjectWrapper>::ObjectWrapper Database::getSkus(){

    auto cursor = m_coll_sku.find({});
    auto listDocs = oatpp::data::mapping::type::List<SkuDto::ObjectWrapper>::createShared();

    for(auto doc : cursor) {        
        listDocs->pushBack(m_mongomapper->readFromDocument<SkuDto::ObjectWrapper>(doc));
    }

    return listDocs;
}

bool Database::deleteSku(v_int32 id){
    auto result = m_coll_sku.delete_one(document{} << "SkuNumber" << id << finalize);

    if(result) {
        OATPP_LOGD(TAG, "Deleted %d sku document with SkuNumber %d", result->deleted_count(), id); 
        return true;
    }

    return false;
}