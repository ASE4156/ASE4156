#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include "Authenticator.h"
#include "db.hpp"
#include "catch.hpp"
#include <fstream> 
#include "api-endpoints/Token.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;

TEST_CASE("Authenticator Function Check") {
    Authenticator authenticator;
    std::string token = authenticator.generateToken();

    SECTION("Validate Token") {
        std::string dbToken = sql_return("SELECT token_id FROM public.token LIMIT 1;"); 
        REQUIRE(authenticator.validateToken(dbToken));
    }
 
    SECTION("Token Generation") {
        REQUIRE(token.length() > 0);
        REQUIRE(!(token == authenticator.generateToken()));
    } 
}

TEST_CASE("Token Endpoint Check") {
    Token tokenEndpoint = Token();

    SECTION("Creation Endpoint Check") {
        std::string clientId = sql_return("SELECT client_id FROM public.client LIMIT 1;");

        http_request mockRequest(methods::POST);
        mockRequest.set_request_uri(U("/token/creation"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("clientId")] = web::json::value::string(U(clientId));
        mockRequest.set_body(requestBody);

        web::http::http_response response = tokenEndpoint.handleCreationRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::OK);
        auto body = response.extract_json().get();
        std::cout << body[U("response")].as_string() << std::endl; 
    }
}

