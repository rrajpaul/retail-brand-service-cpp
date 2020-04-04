cmake_minimum_required(VERSION 3.1)

set(project_name retail-brand-service) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 17)

add_library(${project_name}-lib
        src/AppComponent.hpp
        src/controller/StyleController.hpp
        src/db/Database.hpp
        src/db/Database.cpp
        src/dto/DTOs.hpp
)

## link libs
find_package(libbsoncxx 3.4.0 REQUIRED)
find_package(libmongocxx 3.4.0 REQUIRED)
find_package(oatpp 1.0.0 REQUIRED)
find_package(oatpp-mongocxxmapper 1.0.0 REQUIRED)
find_package(oatpp-swagger 1.0.0 REQUIRED)


target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
        PUBLIC oatpp::oatpp-swagger
        PUBLIC oatpp::oatpp-mongocxxmapper
)
#################################################################
## link postgresql client

add_definitions(-DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res")

include_directories("/usr/local/include/oatpp-1.0.0/oatpp")
include_directories("/usr/local/include/oatpp-1.0.0/oatpp-swagger")
include_directories("/usr/local/include/oatpp-1.0.0/oatpp-mongocxxmapper")

include(FindPkgConfig)

set(ENV{PKG_CONFIG_PATH} "/usr/local/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}") ## change this if needed

pkg_check_modules(PKG_MONGODB REQUIRED libmongocxx)

message("PKG_MONGODB_INCLUDE_DIRS=${PKG_MONGODB_INCLUDE_DIRS}")
message("PKG_MONGODB_LIBRARY_DIRS=${PKG_MONGODB_LIBRARY_DIRS}")
message("PKG_MONGODB_LIBRARIES=${PKG_MONGODB_LIBRARIES}")

target_include_directories(${project_name}-lib
        PUBLIC ${PKG_MONGODB_INCLUDE_DIRS}
)

link_directories(
        ${PKG_MONGODB_LIBRARY_DIRS}
)

target_link_libraries(${project_name}-lib
        PUBLIC ${PKG_MONGODB_LIBRARIES}
)

target_include_directories(${project_name}-lib PUBLIC src)

## add executables

add_executable(${project_name}-exe
        src/App.cpp
        test/app/MyApiTestClient.hpp)
target_link_libraries(${project_name}-exe ${project_name}-lib)
add_dependencies(${project_name}-exe ${project_name}-lib)

add_executable(${project_name}-test
        test/tests.cpp
        test/app/TestComponent.hpp
        test/app/MyApiTestClient.hpp
        test/MyControllerTest.cpp
        test/MyControllerTest.hpp
)

target_link_libraries(${project_name}-test ${project_name}-lib)
add_dependencies(${project_name}-test ${project_name}-lib)

set_target_properties(${project_name}-lib ${project_name}-exe ${project_name}-test PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

enable_testing()
add_test(project-tests ${project_name}-test)
