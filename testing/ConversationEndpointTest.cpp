#include "catch.hpp"
#include "api-endpoints/Conversation.h"
#include "MockChatGPTService.h"
#include <cpprest/http_client.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;

TEST_CASE("Endpoint POST Request Handling") {
    // Mock Service Setup
    MockChatGPTService mockService;
    Conversation conversationEndpoint(mockService);

    SECTION("Successful POST request") {
        http_request mockRequest(methods::POST);
        mockRequest.set_request_uri(U("/llm/text/conversation"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("text")] = web::json::value::string(U("Hello"));
        requestBody[U("prompt_id")] = web::json::value::number(1234);	
        mockRequest.set_body(requestBody);

	web::http::http_response response = conversationEndpoint.handleCompletionRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::OK);
	auto body = response.extract_json().get();
	REQUIRE(body[U("response")].as_string() == U("The 2020 World Series was played in Texas at Globe Life Field in Arlington."));	

        // conversationEndpoint.handleCompletionRequest(mockRequest).then([](web::http::http_response response) {
        //     REQUIRE(response.status_code() == web::http::status_codes::OK);
        //     auto body = response.extract_json().get();
        //     REQUIRE(body[U("response")].as_string() == U("The 2020 World Series was played in Texas at Globe Life Field in Arlington."));
        // }).wait();
    }

    SECTION("Handle invalid request body - missing text field") {
        http_request mockRequest(methods::POST);
        mockRequest.set_request_uri(U("/llm/text/conversation"));
        web::json::value requestBody = web::json::value::object();
        
	// missing text field (prompt_id is optional)
        requestBody[U("prompt_id")] = web::json::value::number(1234);	
        mockRequest.set_body(requestBody);

	web::http::http_response response = conversationEndpoint.handleCompletionRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }
    
    SECTION("Handle invalid request body - invalid text format") {
        http_request mockRequest(methods::POST);
        mockRequest.set_request_uri(U("/llm/text/conversation"));
        web::json::value requestBody = web::json::value::object();
        
	// missing text field (prompt_id is optional)
        requestBody[U("text")] = web::json::value::number(1234);	
        mockRequest.set_body(requestBody);

	web::http::http_response response = conversationEndpoint.handleCompletionRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);

        // conversationEndpoint.handleCompletionRequest(mockRequest).then([](web::http::http_response response) {
        //     REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
        // }).wait();
    }
}