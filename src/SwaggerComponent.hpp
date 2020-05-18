//
//  SwaggerComponent.hpp
//  crud
//
//  Created by Leonid on 7/31/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef SwaggerComponent_hpp
#define SwaggerComponent_hpp

#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp/core/utils/ConversionUtils.hpp"
#include "oatpp/core/macro/component.hpp"
#include "HostInfo.hpp"
/**
 *  Swagger ui is served at
 *  http://host:port/swagger/ui
 */
class SwaggerComponent {
private:
  oatpp::String m_hostAddress;
private:
public:  
  /**
   *  General API docs info
   */

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)([this] {
    
    HostInfo oHost;
    
    oatpp::String hostAddress = oHost.getHostAddress();

    if(hostAddress == "127.0.0.1" || hostAddress == "127.0.1.1") {
        hostAddress = "localhost";
    }

    OATPP_LOGD("Server", "swagger host %s...", hostAddress->c_str());

    oatpp::swagger::DocumentInfo::Builder builder;
    
    builder
    .setTitle("Style entity service")
    .setDescription("MongoDB CRUD API Example project with swagger docs")
    .setVersion("1.0")
    .setContactName("Ivan Ovsyanochka")
    .setContactUrl("https://oatpp.io/")
    
    .setLicenseName("Apache License, Version 2.0")
    .setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0")
    
    //.addServer("this->m_hostAddress", "server on " + this->m_hostAddress);
    .addServer("http://" + hostAddress + ":9000", "server on " + hostAddress);
    
    return builder.build();
    
  }());
  
  
  /**
   *  Swagger-Ui Resources (<oatpp-examples>/lib/oatpp-swagger/res)
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)([] {
    // Make sure to specify correct full path to oatpp-swagger/res folder !!!
    return oatpp::swagger::Resources::loadResources(OATPP_SWAGGER_RES_PATH);
  }());
  
};

#endif /* SwaggerComponent_hpp */
