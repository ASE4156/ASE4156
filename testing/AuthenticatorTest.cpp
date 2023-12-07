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
        auto clientId = sql_return("SELECT client_id FROM public.client LIMIT 1;");

        http_request mockRequest(methods::POST);
        mockRequest.set_request_uri(U("/token/creation"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("clientId")] = web::json::value::number(std::stoi(clientId));
        mockRequest.set_body(requestBody);

        web::http::http_response response = tokenEndpoint.handleCreationRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::OK);
        auto body = response.extract_json().get();
        REQUIRE(body[U("response")].as_string().length() > 0);
    }

    SECTION("Deletion Endpoint Check") {
        http_request mockRequest(methods::POST);
        mockRequest.set_request_uri(U("/token/delete"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U("192jfklsdf;ndfj198dk"));
        mockRequest.set_body(requestBody);

        web::http::http_response response = tokenEndpoint.handleDeletionRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest);
    } 

    SECTION("Get Endpoint Check") {
        auto clientId = sql_return("SELECT client_id FROM public.client LIMIT 1;");

        http_request mockRequest(methods::GET);
        mockRequest.set_request_uri(U("/token/get"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("clientId")] = web::json::value::number(std::stoi(clientId));
        mockRequest.set_body(requestBody);

        web::http::http_response response = tokenEndpoint.handleGetRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::OK); 
        auto body = response.extract_json().get();
        REQUIRE(body[U("response")].as_string().length() > 0);
    }

    SECTION("Validate Endpoint Check") {
        std::string token = sql_return("SELECT token_id FROM public.token LIMIT 1;");

        http_request mockRequest(methods::GET);
        mockRequest.set_request_uri(U("/token/validate"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U(token));
        mockRequest.set_body(requestBody);

        web::http::http_response response = tokenEndpoint.handleGetRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest); 
    }

    SECTION("Get Client Endpoint Check") {
        std::string token = sql_return("SELECT token_id FROM public.token LIMIT 1;");

        http_request mockRequest(methods::GET);
        mockRequest.set_request_uri(U("/token/getClient"));
        web::json::value requestBody = web::json::value::object();
        requestBody[U("token")] = web::json::value::string(U(token));
        mockRequest.set_body(requestBody);

        web::http::http_response response = tokenEndpoint.handleGetRequest(mockRequest);
        REQUIRE(response.status_code() == web::http::status_codes::BadRequest); 
    }
}