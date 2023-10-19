#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <iostream>

using namespace web;
using namespace web::http;
using namespace web::http::client;

std::string base_url = U("http://localhost:8080/");
std::string endpoint1 = U("llm/text/conversation");
std::string endpoint2 = U("revise/user/input");

std::string url1 = base_url+endpoint1;
std::string url2 = base_url+endpoint2;


//functions to get responses

short unsigned int url_post_response_status_code(const std::string& user_input,const std::string& url) {

    http_response response;
    http_client client(base_url+endpoint1);

    json::value requestData;
    requestData[U("text")] = json::value::string(U("user input"));

    http_request request(methods::POST);
    request.headers().set_content_type(U("application/json"));
    // request.headers().set_accept(U("application/json"));
    // request.set_request_uri(base_url+endpoint1);
    request.set_body(requestData);

    response = client.request(request).get();
    return response.status_code();

}


json::value url_post_response_json(const std::string& user_input,const std::string& url) {

    http_response response;
    http_client client(base_url+endpoint1);

    json::value requestData;
    requestData[U("text")] = json::value::string(U("user input"));

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
        REQUIRE(url_post_response_status_code("hi",url1) == status_codes::OK);
    }

    SECTION("Check return data is json object") {
        // Test valid input and verify the status code.
        json::value jsonObj = url_post_response_json("hi",url1);
        REQUIRE(jsonObj.is_object());
    }

    SECTION("Check json contain response field") {
        // Test valid input and verify the status code.
        json::value jsonObj = url_post_response_json("hi",url1);
        REQUIRE(jsonObj.has_field(U("response")));
    }
    
    SECTION("Check json contain response field is string") {
        // Test valid input and verify the status code.
        json::value jsonObj = url_post_response_json("hi",url1);
        REQUIRE(jsonObj[U("response")].is_string());
    }

}



TEST_CASE("Endpoint2 : API Status Code and Response Data Tes", "[Endpoint2]") {

    SECTION("Check status code") {
        // Test valid input and verify the status code.
        REQUIRE(url_post_response_status_code("hi",url2) == status_codes::OK);
    }

    SECTION("Check return data is json object") {
        // Test valid input and verify the status code.
        json::value jsonObj = url_post_response_json("hi",url2);
        REQUIRE(jsonObj.is_object());
    }

    SECTION("Check json contain response field") {
        // Test valid input and verify the status code.
        json::value jsonObj = url_post_response_json("hi",url2);
        REQUIRE(jsonObj.has_field(U("response")));
    }
    
    SECTION("Check json contain response field is string") {
        // Test valid input and verify the status code.
        json::value jsonObj = url_post_response_json("hi",url2);
        REQUIRE(jsonObj[U("response")].is_string());
    }

}


// Scrapwork:
// void url_post_response_print(const std::string& user_input,const std::string& url) {

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
//         std::cout << U("Received response status code: ") << response.status_code() << std::endl;
//         return response.extract_json();
//     }).then([](json::value jsonResp) {
//         std::cout << U("Received response data: ") << jsonResp.serialize() << std::endl;
//     }).wait();

// }


