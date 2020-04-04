#include "Database.hpp"

v_int32 Database::getMaxId(){
  bsoncxx::builder::stream::document sort_doc{};
  sort_doc << "StyleId" << -1 ;
  mongocxx::options::find opts;
  opts.sort(sort_doc.view()).limit(1);

  auto cursor = m_coll.find({}, opts);
  auto style = m_mongomapper->readFromDocument<StyleDto::ObjectWrapper>(*cursor.begin());
  auto maxId = style->StyleId + 10;

  return maxId;
}

StyleDto::ObjectWrapper Database::createStyle(const StyleDto::ObjectWrapper& style){
    auto newId = getMaxId();
    style->StyleId = newId;
    
    auto mongoStyle = m_mongomapper->writeAsDocument(style);
    auto result = m_coll.insert_one(mongoStyle.view());
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
        auto result =   m_coll.replace_one(
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
    auto result = m_coll.find_one({document{} << "StyleId" << id << finalize});
    if(result) {
        auto docView = result->view();
        style = m_mongomapper->readFromDocument<StyleDto::ObjectWrapper>(docView);

        OATPP_LOGD(TAG, "Found style with id %d", style->StyleId->getValue()); 
    }

    return style;
}

oatpp::data::mapping::type::List<SkuDto::ObjectWrapper>::ObjectWrapper Database::getSkuByStyleId(v_int32 id){   
    auto result = m_coll.find_one({document{} << "StyleId" << id << finalize});
    auto listDocs = oatpp::data::mapping::type::List<SkuDto::ObjectWrapper>::createShared();
    if(result) {
        OATPP_LOGD(TAG, "Found Style with id %d", id); 
        auto cursor_sku = m_coll_sku.find(document{} << "StyleId" << id << finalize);
        
        for(auto doc : cursor_sku) {        
            listDocs->pushBack(m_mongomapper->readFromDocument<SkuDto::ObjectWrapper>(doc));
        }   
    }

    return listDocs;
}

oatpp::data::mapping::type::List<StyleDto::ObjectWrapper>::ObjectWrapper Database::getStyles(){

    auto cursor = m_coll.find({});
    auto listDocs = oatpp::data::mapping::type::List<StyleDto::ObjectWrapper>::createShared();

    for(auto doc : cursor) {        
        listDocs->pushBack(m_mongomapper->readFromDocument<StyleDto::ObjectWrapper>(doc));
    }

    return listDocs;
}

bool Database::deleteStyle(v_int32 id){
    m_coll = m_db[COLLECTION_STYLE];
    auto result = m_coll.delete_one(document{} << "StyleId" << id << finalize);

    if(result) {
        return true;
    }

    return false;
}