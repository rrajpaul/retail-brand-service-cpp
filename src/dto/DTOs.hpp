//
//  StyleDto.hpp
//  crud
//
//  Created by Leonid on 3/13/18.
//  Modified by Ryan Rajpaul 01/31/2020
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef DTOs_hpp
#define DTOs_hpp

#include "oatpp/core/data/mapping/type/Object.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp-mongocxxmapper/ObjectMapper.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class SkuDto : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(SkuDto, Object)
  DTO_FIELD(Int32, SkuNumber,"SkuNumber");
  DTO_FIELD(Int32, StyleId);
  DTO_FIELD(String, Size, "Size");
  DTO_FIELD(String, Color, "Color");
  DTO_FIELD(Int32, InStock, "InStock");
  DTO_FIELD(Float64, Price, "Price");
};

class StyleDto : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(StyleDto, Object)
  DTO_FIELD(Int32, StyleId);
  DTO_FIELD(String, Manufacturer, "Manufacturer");
  DTO_FIELD(String, Brand, "Brand");
  DTO_FIELD(String, Category, "Category");
  DTO_FIELD(String, Type, "Type");
  DTO_FIELD(String, Description, "Description");
};



#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */
