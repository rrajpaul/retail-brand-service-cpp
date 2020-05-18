# retail-brand-service-cpp [![Build Status](https://dev.azure.com/lganzzzo/lganzzzo/_apis/build/status/example-crud?branchName=master)](https://dev.azure.com/lganzzzo/lganzzzo/_build?definitionId=9?branchName=master)

Example project how-to create basic MongoDB CRUD endpoints and document them with Swagger-UI and OpenApi 3.0.0

See more:

- [Oat++ Website](https://oatpp.io/)
- [Oat++ Github Repository](https://github.com/oatpp/oatpp)
- [Get Started](https://oatpp.io/docs/start)

## Overview

This project is using [oatpp](https://github.com/oatpp/oatpp) and [oatpp-swagger](https://github.com/oatpp/oatpp-swagger) modules.

### Project layout

```
|- CMakeLists.txt                        // projects CMakeLists.txt
|- src/
|   |
|   |- controller/                       // Folder containing StyleController where all endpoints are declared
|   |- db/                               // Folder with database mock
|   |- dto/                              // DTOs are declared here
|   |- SwaggerComponent.hpp              // Swagger-UI config
|   |- AppComponent.hpp                  // Service config
|   |- App.cpp                           // main() is here
|
|- test/                                 // test folder
|- utility/install-oatpp-modules.sh      // utility script to install required oatpp-modules.
```

---

### Build and Run

#### Using CMake

**Requires**

- `oatpp` and `oatpp-swagger` and `bsoncxx` and `mongocxx` modules installed. You may run `utility/install-oatpp-modules.sh` 
script to install required oatpp modules.

```
$ mkdir build && cd build
$ cmake ..
$ make 
$ ./retail-brand-service-cpp-exe        # - run application.
```

#### In Docker

```
$ run script `build-docker-images.sh` in root of project this builds 2 images one for mongodb and the other for the wen app `retail-brand-service-cpp`
Test the web app in web browser 172.17.0.3:9000 or verify the ip address for `retail_brand_service1` using `docker inspect retail_brand_service1`

```

---

### Endpoints declaration

#### Create Style

```c++
ENDPOINT_INFO(createStyle) {
  info->summary = "Create new Style";
  info->addConsumes<StyleDto::ObjectWrapper>("application/json");
  info->addResponse<StyleDto::ObjectWrapper>(Status::CODE_200, "application/json");
}
ENDPOINT("POST", "demo/api/styles", createStyle,
         BODY_DTO(StyleDto::ObjectWrapper, styleDto)) {
  return createDtoResponse(Status::CODE_200, m_database->createStyle(styleDto));
}
```

#### Update Style

```c++
ENDPOINT_INFO(putStyle) {
  info->summary = "Update Style by styleId";
  info->addConsumes<StyleDto::ObjectWrapper>("application/json");
  info->addResponse<StyleDto::ObjectWrapper>(Status::CODE_200, "application/json");
  info->addResponse<String>(Status::CODE_404, "text/plain");
}
ENDPOINT("PUT", "demo/api/styles/{styleId}", putStyle,
         PATH(Int32, styleId),
         BODY_DTO(StyleDto::ObjectWrapper, styleDto)) {
  styleDto->id = styleId;
  return createDtoResponse(Status::CODE_200, m_database->updateStyle(styleDto));
}
```

#### Get one Style

```c++
ENDPOINT_INFO(getStyleById) {
  info->summary = "Get one Style by styleId";
  info->addResponse<StyleDto::ObjectWrapper>(Status::CODE_200, "application/json");
  info->addResponse<String>(Status::CODE_404, "text/plain");
}
ENDPOINT("GET", "demo/api/styles/{styleId}", getStyleById,
         PATH(Int32, styleId)) {
  auto style = m_database->getStyleById(styleId);
  OATPP_ASSERT_HTTP(style, Status::CODE_404, "Style not found");
  return createDtoResponse(Status::CODE_200, style);
}
```

#### Get list of styles

```c++
ENDPOINT_INFO(getStyles) {
  info->summary = "get all stored styles";
  info->addResponse<List<StyleDto::ObjectWrapper>::ObjectWrapper>(Status::CODE_200, "application/json");
}
ENDPOINT("GET", "demo/api/styles", getStyles) {
  return createDtoResponse(Status::CODE_200, m_database->getStyles());
}
```

#### Delete Style
```c++
ENDPOINT_INFO(deleteStyle) {
  info->summary = "Delete Style by styleId";
  info->addResponse<String>(Status::CODE_200, "text/plain");
  info->addResponse<String>(Status::CODE_404, "text/plain");
}
ENDPOINT("DELETE", "demo/api/styles/{styleId}", deleteStyle,
         PATH(Int32, styleId)) {
  bool success = m_database->deleteStyle(styleId);
  OATPP_ASSERT_HTTP(success, Status::CODE_417, "Style not deleted. Perhaps no such Style in the Database");
  return createResponse(Status::CODE_200, "Style successfully deleted");
}  
```
