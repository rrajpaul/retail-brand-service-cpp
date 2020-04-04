//
//  StyleController.hpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef StyleController_hpp
#define StyleController_hpp

#include "../db/Database.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

/**
 *  EXAMPLE ApiController
 *  Basic examples of howto create ENDPOINTs
 *  More details on oatpp.io
 */
class StyleController : public oatpp::web::server::api::ApiController {
public:
  StyleController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
private:
  
  /**
   *  Inject Database component
   */
  OATPP_COMPONENT(std::shared_ptr<Database>, m_database);
public:
  
  /**
   *  Inject @objectMapper component here as default parameter
   *  Do not return bare Controllable* object! use shared_ptr!
   */
  static std::shared_ptr<StyleController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                 objectMapper)){
    return std::make_shared<StyleController>(objectMapper);
  }
  
  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)
  
  /**
   *  Insert Your endpoints here !!!
   */
  
  ENDPOINT_INFO(root) {
    info->summary = "Index.html page";
    info->addResponse<StyleDto::ObjectWrapper>(Status::CODE_200, "text/html");
  }
  ENDPOINT("GET", "/", root) {
    const char* html =
    "<html lang='en'>"
    "<head>"
    "<meta charset=utf-8/>"
    "</head>"
    "<body>"
    "<p>MongoDB example project!</p>"
    "<a href='swagger/ui'>Checkout Swagger-UI page</a>"
    "</body>"
    "</html>";
    auto response = createResponse(Status::CODE_200, html);
    response->putHeader(Header::CONTENT_TYPE, "text/html");
    return response;
  }
  
  ADD_CORS(createStyle)
  ENDPOINT_INFO(createStyle) {
    info->summary = "Create new Style";
    info->addConsumes<StyleDto::ObjectWrapper>("application/json");
    info->addResponse<StyleDto::ObjectWrapper>(Status::CODE_200, "application/json");
  }
  ENDPOINT("POST", "demo/api/styles", createStyle,
           BODY_DTO(StyleDto::ObjectWrapper, styleDto)) {
    return createDtoResponse(Status::CODE_200, m_database->createStyle(styleDto));
  }
  
  ADD_CORS(putStyle)
  ENDPOINT_INFO(putStyle) {
    // general
    info->summary = "Update Style by StyleId";
    info->addConsumes<StyleDto::ObjectWrapper>("application/json");
    info->addResponse<StyleDto::ObjectWrapper>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    // params specific
    info->pathParams["styleId"].description = "Style Identifier";
  }
  ENDPOINT("PUT", "demo/api/styles/{styleId}", putStyle,   \
           PATH(Int32, styleId),        
           BODY_DTO(StyleDto::ObjectWrapper, styleDto)) {
           styleDto->StyleId = styleId;
    return createDtoResponse(Status::CODE_200, m_database->updateStyle(styleDto));
  }
  
  ADD_CORS(getStyleById)
  ENDPOINT_INFO(getStyleById) {
    // general
    info->summary = "Get Style by styleId";
    info->addResponse<StyleDto::ObjectWrapper>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    // params specific
    info->pathParams["styleId"].description = "Style Identifier";
  }
  ENDPOINT("GET", "demo/api/styles/{styleId}", getStyleById,
           PATH(Int32, styleId)) {
    auto style = m_database->getStyleById(styleId);
    OATPP_ASSERT_HTTP(style, Status::CODE_404, "Style not found");
    return createDtoResponse(Status::CODE_200, style);
  }

  ADD_CORS(getSkuByStyleId)
  ENDPOINT_INFO(getSkuByStyleId) {
    // general
    info->summary = "Get Sku by styleId";
    info->addResponse<StyleDto::ObjectWrapper>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    // params specific
    info->pathParams["styleId"].description = "Style Identifier";
  }
  ENDPOINT("GET", "demo/api/styles/{styleId}/sku", getSkuByStyleId,
           PATH(Int32, styleId)) {
    auto sku = m_database->getSkuByStyleId(styleId);
    OATPP_ASSERT_HTTP(sku, Status::CODE_404, "Skus for Style not found");
    return createDtoResponse(Status::CODE_200, sku);
  }
  
  ADD_CORS(getStyles)
  ENDPOINT_INFO(getStyles) {
    info->summary = "get all stored styles";
    info->addResponse<oatpp::data::mapping::type::List<StyleDto::ObjectWrapper>::ObjectWrapper>(Status::CODE_200, "application/json");
  }
  ENDPOINT("GET", "demo/api/styles", getStyles) {
    return createDtoResponse(Status::CODE_200, m_database->getStyles());
  }

  ADD_CORS(deleteStyle)
  ENDPOINT_INFO(deleteStyle) {
    // general
    info->summary = "Delete Style by styleId";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    // params specific
    info->pathParams["styleId"].description = "Style Identifier";
  }
  ENDPOINT("DELETE", "demo/api/styles/{styleId}", deleteStyle,
           PATH(Int32, styleId)) {
    bool success = m_database->deleteStyle(styleId);
    OATPP_ASSERT_HTTP(success, Status::CODE_417, "Style not deleted. Perhaps no such Style in the Database");
    return createResponse(Status::CODE_200, "Style successfully deleted");
  }
  
  /**
   *  Finish ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_END(ApiController)
  
};

#endif /* StyleController_hpp */
