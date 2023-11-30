#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "catch.hpp"
#include "db.hpp"
#include "service/Authenticator.h"

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <iostream>

using namespace web;
using namespace web::http;
using namespace web::http::client;

std::string base_url = U("http://localhost:8080/");
std::string endpoint1 = U("llm/text/conversation");
std::string endpoint2 = U("revise/user/input");

std::string url1 = base_url + endpoint1;
std::string url2 = base_url + endpoint2;

// functions to get responses

short unsigned int url_post_response_status_code(const std::string &json_key,
                                                 const std::string &json_val,
                                                 const std::string &url) {

  http_response response;
  http_client client(base_url + endpoint1);

  json::value requestData;
  requestData[U(json_key)] = json::value::string(U(json_val));

  http_request request(methods::POST);
  request.headers().set_content_type(U("application/json"));
  // request.headers().set_accept(U("application/json"));
  // request.set_request_uri(base_url+endpoint1);
  request.set_body(requestData);

  response = client.request(request).get();
  return response.status_code();
}

json::value url_post_response_json(const std::string &json_key,
                                   const std::string &json_val,
                                   const std::string &url) {

  http_response response;
  http_client client(base_url + endpoint1);

  json::value requestData;
  requestData[U(json_key)] = json::value::string(U(json_val));

  http_request request(methods::POST);
  request.headers().set_content_type(U("application/json"));
  // request.headers().set_accept(U("application/json"));
  // request.set_request_uri(base_url+endpoint1);
  request.set_body(requestData);

  response = client.request(request).get();
  return response.extract_json().get();
}

TEST_CASE("Endpoint1 : API Status Code and Response Data Tes", "[Endpoint1]") {

  SECTION("Check status code") {
    // Test valid input and verify the status code.
    REQUIRE(url_post_response_status_code("text", "hi", url1) == status_codes::OK);
  }

  SECTION("Check return data is json object") {
    // Test valid input and verify the status code.
    json::value jsonObj = url_post_response_json("text", "hi", url1);
    REQUIRE(jsonObj.is_object());
  }

  SECTION("Check json contain response field") {
    // Test valid input and verify the status code.
    json::value jsonObj = url_post_response_json("text", "hi", url1);
    REQUIRE(jsonObj.has_field(U("response")));
  }

  SECTION("Check json contain response field is string") {
    // Test valid input and verify the status code.
    json::value jsonObj = url_post_response_json("text", "hi", url1);
    REQUIRE(jsonObj[U("response")].is_string());
  }
}

TEST_CASE("Endpoint2 : API Status Code and Response Data Tes", "[Endpoint2]") {

  SECTION("Check status code") {
    // Test valid input and verify the status code.
    REQUIRE(url_post_response_status_code("text", "hi", url2) == status_codes::OK);
  }

  SECTION("Check return data is json object") {
    // Test valid input and verify the status code.
    json::value jsonObj = url_post_response_json("text", "hi", url2);
    REQUIRE(jsonObj.is_object());
  }

  SECTION("Check json contain response field") {
    // Test valid input and verify the status code.
    json::value jsonObj = url_post_response_json("text", "hi", url2);
    REQUIRE(jsonObj.has_field(U("response")));
  }

  SECTION("Check json contain response field is string") {
    // Test valid input and verify the status code.
    json::value jsonObj = url_post_response_json("text", "hi", url2);
    REQUIRE(jsonObj[U("response")].is_string());
  }
}

TEST_CASE("Authentication : Authentication Functions Test", "[Authentication]") {

  SECTION("Get and Validate Token") {
    http_response response;
    http_client client(U("http://localhost:8080/token/get"));

    json::value requestData;
    requestData[U("clientId")] = json::value::number(1234);

    http_request request(methods::GET);
    request.headers().set_content_type(U("application/json"));
    request.set_body(requestData);

    response = client.request(request).get();
    auto jsonValue = response.extract_json().get();
    std::string token = jsonValue[U("response")].as_string();
    // Test valid input and verify the status code.
    Authenticator authenticator;
    REQUIRE(authenticator.validateToken(token));
  }

  SECTION("Generate Token") {
    Authenticator authenticator;
    REQUIRE(authenticator.generateToken().length() > 0);
  }

  SECTION("Create, Get, and Delete Token") {
    // Creation Endpoint
    http_response creationResponse;
    http_client creationClient(U("http://localhost:8080/token/creation"));

    json::value creationRequestData;
    creationRequestData[U("clientId")] = json::value::number(1234);

    http_request creationRequest(methods::GET);
    creationRequest.headers().set_content_type(U("application/json"));
    creationRequest.set_body(creationRequestData);

    creationResponse = creationClient.request(creationRequest).get();
    auto creationJsonValue = creationResponse.extract_json().get();
    std::string returnedToken = creationJsonValue[U("response")].as_string();
    std::string createdToken = sql_return("SELECT token_id FROM public.token WHERE client_id=1234 ORDER BY created_at DESC LIMIT 1;");
    REQUIRE(createdToken == returnedToken);
    
    // Get Token Endpoint
    http_response getResponse;
    http_client getClient(U("http://localhost:8080/token/get"));

    json::value getRequestData;
    getRequestData[U("clientId")] = json::value::number(1234);

    http_request getRequest(methods::GET);
    getRequest.headers().set_content_type(U("application/json"));
    getRequest.set_body(getRequestData);

    getResponse = getClient.request(getRequest).get();
    auto getJsonValue = getResponse.extract_json().get();
    std::string token = getJsonValue[U("response")].as_string();
    REQUIRE(token == createdToken);

    // Delete Token Endpoint
    http_response deletionResponse;
    http_client deletionClient(U("http://localhost:8080/token/delete"));

    json::value deletionRequestData;
    deletionRequestData[U("token")] = json::value::string(U(token));

    http_request deletionRequest(methods::POST);
    deletionRequest.headers().set_content_type(U("application/json"));
    deletionRequest.set_body(deletionRequestData);

    deletionResponse = deletionClient.request(deletionRequest).get();

    std::string tokenExists = sql_return("SELECT token_id FROM public.token WHERE token_id='" + token + "'");
    REQUIRE(tokenExists.length() == 0);
  }
}

// Scrapwork:
// void url_post_response_print(const std::string& user_input,const std::string&
// url) {

//     http_response response;
//     http_client client(base_url+endpoint1);

//     json::value requestData;
//     requestData[U("text")] = json::value::string(U("user input"));

//     http_request request(methods::POST);
//     request.headers().set_content_type(U("application/json"));
//     // request.headers().set_accept(U("application/json"));
//     // request.set_request_uri(base_url+endpoint1);
//     request.set_body(requestData);

//     client.request(request).then([](http_response response) {
//         std::cout << U("Received response status code: ") <<
//         response.status_code() << std::endl; return response.extract_json();
//     }).then([](json::value jsonResp) {
//         std::cout << U("Received response data: ") << jsonResp.serialize() <<
//         std::endl;
//     }).wait();

// }