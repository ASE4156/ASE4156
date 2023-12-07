#include "catch.hpp"
#include "api-endpoints/Prompt.h"
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pqxx/pqxx>
#include "db.h"

std::string sql_return(const std::string& query);

using namespace web;
using namespace web::http;
using namespace web::http::client;

void mockclient(){
    std::string delete2 = R"(
        INSERT INTO client (client_id, client_name, client_email, client_password)
        VALUES 
            (9999, 'mock_client', 'mock_client@email.com', 'mock1234')
    )";
    std::string ret3 = sql_return(delete2);
}

void mockprompt(){
    std::string delete1 = R"(
        INSERT INTO prompt (prompt_id, prompt_name, prompt_description, prompt_content, client_id)
        VALUES 
            (999999, 'mock_prompt', 'mock_des', 'mock_content', 0)
   )";
    std::string ret2 = sql_return(delete1);
}

void mocktoken(){
    std::string delete1 = R"(
        INSERT INTO token (client_id, token_id)
        VALUES 
            (9999, '77d9de23-f88f-4add-bc08-88260189cb52')
    )";
    std::string ret2 = sql_return(delete1);
}

void deleteclient(){
    std::string delete2 = R"(
        DELETE FROM client
        WHERE client_id = 9999;
    )";
    std::string ret3 = sql_return(delete2);
}

void deleteprompt1(){
    std::string delete1 = R"(
        DELETE FROM prompt
        WHERE prompt_id = 999999;
    )";
    std::string ret2 = sql_return(delete1);
}

void deleteprompt2(){
    std::string delete1 = R"(
        DELETE FROM prompt
        WHERE prompt_id = 1000000;
    )";
    std::string ret2 = sql_return(delete1);
}

void deletetoken(){
    std::string delete1 = R"(
        DELETE FROM token
        WHERE token_id = '77d9de23-f88f-4add-bc08-88260189cb52';
    )";
    std::string ret2 = sql_return(delete1);
}


TEST_CASE("Endpoint Prompt Post") {
    // Mock Service Setup
    Prompt promptEndpoint;
    mockclient();
    mockprompt();
    mocktoken();

    SECTION("Successful POST request") {
        http_request mockRequest(methods::POST);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz"));
        requestBody[U("client_id")] = web::json::value::number(9999);	
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
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
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
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
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
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
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
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz"));
        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePostRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    deleteprompt2();
    deleteprompt1();
    deletetoken();
    deleteclient();
}

TEST_CASE("Endpoint Prompt Put") {
    // Mock Service Setup
    Prompt promptEndpoint;
    mockclient();
    mockprompt();
    mocktoken();

    SECTION("Successful PUT request") {
        http_request mockRequest(methods::PUT);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz_update"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz_update"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz_update"));
        requestBody[U("client_id")] = web::json::value::number(9999);	
        requestBody[U("prompt_id")] = web::json::value::number(999999);	
        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePutRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::OK);
    }


    SECTION("Invalid prompt_id request") {
        http_request mockRequest(methods::PUT);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz_update"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz_update"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz_update"));
        requestBody[U("client_id")] = web::json::value::number(9999);	
        requestBody[U("prompt_id")] = web::json::value::number(123456789);	
        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePutRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    SECTION("Missing token Put request") {
        http_request mockRequest(methods::PUT);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz"));
        requestBody[U("client_id")] = web::json::value::number(1234);	
        requestBody[U("prompt_id")] = web::json::value::number(7);	

        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePutRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    SECTION("Missing prompt_name Put request") {
        http_request mockRequest(methods::PUT);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz"));
        requestBody[U("client_id")] = web::json::value::number(1234);
        requestBody[U("prompt_id")] = web::json::value::number(7);	
	
        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePutRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    SECTION("Missing prompt_description Put request") {
        http_request mockRequest(methods::PUT);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz"));
        requestBody[U("client_id")] = web::json::value::number(1234);
        requestBody[U("prompt_id")] = web::json::value::number(7);	
	
        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePutRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    SECTION("Missing prompt_content Put request") {
        http_request mockRequest(methods::PUT);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz"));
        requestBody[U("client_id")] = web::json::value::number(1234);	
        requestBody[U("prompt_id")] = web::json::value::number(7);	

        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePutRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    SECTION("Missing client_id Put request") {
        http_request mockRequest(methods::PUT);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz"));
        requestBody[U("prompt_id")] = web::json::value::number(7);	

        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePutRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    SECTION("Missing prompt_id Put request") {
        http_request mockRequest(methods::PUT);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz"));
        requestBody[U("client_id")] = web::json::value::number(1234);	

        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePutRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    
    deleteprompt1();
    deletetoken();
    deleteclient();

}


TEST_CASE("Endpoint Prompt Delete") {
    // Mock Service Setup
    Prompt promptEndpoint;
    // Mock prompt
    mockclient();
    mocktoken();
    mockprompt();

    SECTION("Successful Delete request") {
        http_request mockRequest(methods::DEL);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
        requestBody[U("prompt_id")] = web::json::value::number(999999);	
        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handleDeleteRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::OK);
    }

    SECTION("Invalid prompt_id Delete request") {
        http_request mockRequest(methods::DEL);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
        requestBody[U("prompt_id")] = web::json::value::number(12345678);	
        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handleDeleteRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    SECTION("Missing prompt_id Delete request") {
        http_request mockRequest(methods::DEL);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz"));
        requestBody[U("client_id")] = web::json::value::number(999999);	

        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handlePutRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    deleteprompt1();
    deletetoken();
    deleteclient();
}


TEST_CASE("Endpoint Prompt Get") {
    // Mock Service Setup
    Prompt promptEndpoint;
    mockclient();
    mockprompt();
    mocktoken();

    SECTION("Successful GET request") {
        http_request mockRequest(methods::GET);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
        requestBody[U("prompt_id")] = web::json::value::number(999999);	
        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handleGetRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::OK);
    }

    SECTION("Missing prompt_id Get request") {
        http_request mockRequest(methods::GET);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));

        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handleGetRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    SECTION("Missing token Get request") {
        http_request mockRequest(methods::GET);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("prompt_id")] = web::json::value::number(999999);	

        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handleGetRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    deleteprompt1();
    deletetoken();
    deleteclient();
}


TEST_CASE("Endpoint Prompt Get Client") {
    // Mock Service Setup
    Prompt promptEndpoint;
    mockclient();
    mockprompt();
    mocktoken();

    SECTION("Successful GET request") {
        http_request mockRequest(methods::GET);
        mockRequest.set_request_uri(U("/prompt/client_id"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
        requestBody[U("client_id")] = web::json::value::number(9999);	
        mockRequest.set_body(requestBody);

	    web::http::http_response response = promptEndpoint.handleGetClientRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::OK);
    }


    SECTION("Successful GET request") {
        http_request mockRequest(methods::GET);
        mockRequest.set_request_uri(U("/prompt/client_id"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
        requestBody[U("client_id")] = web::json::value::number(12345678);	
        mockRequest.set_body(requestBody);

	    web::http::http_response response = promptEndpoint.handleGetClientRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    SECTION("Missing client Get Client request") {
        http_request mockRequest(methods::GET);
        mockRequest.set_request_uri(U("/prompt/client_id"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));

        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handleGetClientRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }
    
    SECTION("Missing token Get Client request") {
        http_request mockRequest(methods::GET);
        mockRequest.set_request_uri(U("/prompt/client_id"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));

        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handleGetClientRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    }

    deleteprompt1();
    deletetoken();
    deleteclient();
}
