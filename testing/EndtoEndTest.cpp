#include "catch.hpp"
#include "api-endpoints/Prompt.h"
#include "api-endpoints/User.h"
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pqxx/pqxx>
#include "db.h"
#include "service/Authenticator.h"


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

void deletetoken(){
    std::string delete1 = R"(
        DELETE FROM token
        WHERE token_id = '77d9de23-f88f-4add-bc08-88260189cb52';
    )";
    std::string ret2 = sql_return(delete1);
}

int global_prompt_id = -1; // Default value or an indicator
int global_client_id = -1; // Default value or an indicator
std::string global_token = ""; // Default value or an indicator

TEST_CASE("End to END Test") {
    User user;
    SECTION("1.Register for client") {

        http_request mockRequest(methods::GET);
        mockRequest.set_request_uri(U("/user/creation"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("clientName")] = web::json::value::string(U("mock"));
        requestBody[U("clientEmail")] = web::json::value::string(U("mock@example.com"));
        requestBody[U("clientPassword")] = web::json::value::string(U("1234"));
        mockRequest.set_body(requestBody);

	    web::http::http_response response = user.handleCreationRequest(mockRequest);
        int global_client_id = response.extract_json().get()[U("clientId")].as_integer();
        // REQUIRE(global_client_id==11);
        REQUIRE(response.status_code() == web::http::status_codes::OK);

        mockclient();
        mocktoken();
        global_client_id = 9999;
        global_token = U("77d9de23-f88f-4add-bc08-88260189cb52");

    }    
    // SECTION("2.creating tokens") {} 
    // SECTION("3.GET tokens") {}    
    // SECTION("4.GET all tokens for client") {}    
    // SECTION("5.Validate tokens") {} 
    

    Prompt promptEndpoint;

    SECTION("6. Creating Prompt1") {
        http_request mockRequest(methods::POST);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(global_token);
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz"));
        requestBody[U("client_id")] = web::json::value::number(global_client_id);	
        mockRequest.set_body(requestBody);

	    web::http::http_response response = promptEndpoint.handlePostRequest(mockRequest);
        std::string resp = response.extract_json().get()[U("prompt_id")].as_string();
        global_prompt_id = std::stoi(resp);
        // REQUIRE(global_prompt_id==34);
        REQUIRE(response.status_code() == web::http::status_codes::OK);
    }

    SECTION("7.GET prompt for client_id") {
        http_request mockRequest(methods::GET);
        mockRequest.set_request_uri(U("/prompt/client_id"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(global_token);
        requestBody[U("client_id")] = web::json::value::number(global_client_id);	
        mockRequest.set_body(requestBody);

	    web::http::http_response response = promptEndpoint.handleGetClientRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::OK);
    }

    SECTION("8.GET prompt_id") {
        http_request mockRequest(methods::GET);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(global_token);
        requestBody[U("prompt_id")] = web::json::value::number(global_prompt_id);	
        mockRequest.set_body(requestBody);

	web::http::http_response response = promptEndpoint.handleGetRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::OK);
    } 

    SECTION("9.EDIT prompt") {
        http_request mockRequest(methods::PUT);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(global_token);
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz_update"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz_update"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz_update"));
        requestBody[U("client_id")] = web::json::value::number(global_client_id);	
        requestBody[U("prompt_id")] = web::json::value::number(global_prompt_id);	
        mockRequest.set_body(requestBody);

    } 

    // SECTION("10.Conversation") {} 

    // SECTION("11.delete tokens") {}    

    SECTION("12.delete prompt") {

        http_request mockRequest(methods::DEL);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(global_token);
        requestBody[U("prompt_id")] = web::json::value::number(global_prompt_id);	
        mockRequest.set_body(requestBody);

        web::http::http_response response = promptEndpoint.handleDeleteRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::OK);

        deletetoken();
        deleteclient();
    }    

}

