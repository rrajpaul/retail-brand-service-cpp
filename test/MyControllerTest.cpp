//
// Created by Leonid  on 2019-06-07.
//

#include "MyControllerTest.hpp"

#include "controller/RetailBrandController.hpp"

#include "app/MyApiTestClient.hpp"
#include "app/TestComponent.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"

#include "oatpp-test/web/ClientServerTestRunner.hpp"

void MyControllerTest::onRun() {

  /* Register test components */
  TestComponent component;

  /* Create client-server test runner */
  oatpp::test::web::ClientServerTestRunner runner;

  /* Add MyController endpoints to the router of the test server */
  runner.addController(std::make_shared<StyleController>());

  /* Run test */
  runner.run([this, &runner] {

    /* Get client connection provider for Api Client */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, clientConnectionProvider);

    /* Get object mapper component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

    /* Create http request executor for Api Client */
    auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(clientConnectionProvider);

    /* Create Test API client */
    auto client = MyApiTestClient::createShared(requestExecutor, objectMapper);

    /* Call server API */
    /* Call root endpoint of MyController */
    auto response = client->getRoot();

    /* Assert that server responds with 200 */
    OATPP_ASSERT(response->getStatusCode() == 200);

    /* Read response body as MessageDto */
    auto style = response->readBodyToDto<StyleDto>(objectMapper.get());

    /* Assert that received message is as expected */
    OATPP_ASSERT(style);
    OATPP_ASSERT(style->StyleId->getValue() == 5300);
    OATPP_ASSERT(style->Description == "Sport Tshirt");

  }, std::chrono::minutes(10) /* test timeout */);

  /* wait all server threads finished */
  std::this_thread::sleep_for(std::chrono::seconds(1));

}
