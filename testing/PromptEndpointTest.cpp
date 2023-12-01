#include "catch.hpp"
#include "api-endpoints/Prompt.h"
#include <cpprest/http_client.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;

TEST_CASE("Endpoint Prompt Post") {
    // Mock Service Setup
    Prompt promptEndpoint;

    SECTION("Successful POST request") {
        http_request mockRequest(methods::POST);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb53"));
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz"));
        requestBody[U("client_id")] = web::json::value::number(1234);	
        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePostRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::OK);
    }

    SECTION("Missing token POST request") {
        http_request mockRequest(methods::POST);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz"));
        requestBody[U("client_id")] = web::json::value::number(1234);	
        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePostRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    SECTION("Missing prompt_name POST request") {
        http_request mockRequest(methods::POST);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb53"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz"));
        requestBody[U("client_id")] = web::json::value::number(1234);	
        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePostRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    SECTION("Missing prompt_description POST request") {
        http_request mockRequest(methods::POST);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb53"));
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz"));
        requestBody[U("client_id")] = web::json::value::number(1234);	
        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePostRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    SECTION("Missing prompt_content POST request") {
        http_request mockRequest(methods::POST);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb53"));
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz"));
        requestBody[U("client_id")] = web::json::value::number(1234);	
        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePostRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    SECTION("Missing client_id POST request") {
        http_request mockRequest(methods::POST);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb53"));
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz"));
        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePostRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    SECTION("Invalid client_id POST request") {
        http_request mockRequest(methods::POST);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb53"));
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz"));
        requestBody[U("client_id")] = web::json::value::number(100000);	
        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePostRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::OK);
    }
}
