#include "catch.hpp"
#include "api-endpoints/Prompt.h"
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

int global_prompt_id = -1; // Default value or an indicator
int global_client_id = -1; // Default value or an indicator

TEST_CASE("End to END Test") {
    // SECTION("1.Register for client") {}    
    // SECTION("2.creating tokens") {} 
    // SECTION("3.GET tokens") {}    
    // SECTION("4.GET all tokens for client") {}    
    // SECTION("5.Validate tokens") {} 
    

    Prompt promptEndpoint;
    mockclient();
    mocktoken();
    global_client_id = 9999;

    SECTION("6. Creating Prompt1") {
        http_request mockRequest(methods::POST);
        mockRequest.set_request_uri(U("/prompt"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("77d9de23-f88f-4add-bc08-88260189cb52"));
        requestBody[U("prompt_name")] = web::json::value::string(U("test_name_wz"));
        requestBody[U("prompt_description")] = web::json::value::string(U("test_description_wz"));
        requestBody[U("prompt_content")] = web::json::value::string(U("test_prompt_content_wz"));
        requestBody[U("client_id")] = web::json::value::number(global_client_id);	
        mockRequest.set_body(requestBody);

	    web::http::http_response response = promptEndpoint.handlePostRequest(mockRequest);
        std::string resp = response.extract_json().get()[U("prompt_id")].as_string();
        int prompt_id = std::stoi(resp);
        // REQUIRE(prompt_id==34);
        REQUIRE(response.status_code() == web::http::status_codes::OK);
    }

    deletetoken();
    deleteclient();

    // SECTION("6.GET prompt for client_id") {} 
    // SECTION("7.GET prompt_id") {} 
    // SECTION("8.EDIT prompt") {} 

    // SECTION("9.Conversation") {} 

    // SECTION("10.delete tokens") {}    
    // SECTION("11.delete prompt") {}    

}

