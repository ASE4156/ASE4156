#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include "Token.h"
#include "Authenticator.h"
#include "db.hpp"

using namespace web::http;

Token::Token() {}

void Token::handleCreationRequest(http_request request) { 
    // Parse JSON request
    auto json_value = request.extract_json().get();
    // Ensure the JSON value contains a "text" field.
    if (!json_value.has_field(U("clientId"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'clientId' field in JSON request."));
    	return;
    }
    auto clientId = json_value[U("clientId")].as_integer();
    std::string clientIdStr = std::to_string(clientId);

    // Parse JSON request
    Authenticator authenticator;
    std::string uuid = authenticator.generateToken();

    // Create response JSON
    json::value response;
    response[U("response")] = json::value::string(utility::conversions::to_string_t(uuid));

    sql_return("INSERT INTO public.token(token_id, client_id) VALUES ('" + uuid + "', " + clientIdStr + ")");

    // Send back the response
    request.reply(status_codes::OK, response);
}

void Token::handleDeletionRequest(http_request request) {
    // Parse JSON request
    auto json_value = request.extract_json().get();
    // Ensure the JSON value contains a "text" field.
    if (!json_value.has_field(U("token")) || !json_value[U("token")].is_string()) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'token' field in JSON request."));
    	return;
    }

    auto token = json_value[U("token")].as_string();

    Authenticator authenticator;
    if (!authenticator.validateToken(token)) {
    	request.reply(status_codes::BadRequest, U("Invalid 'token' given."));
    	return;
    }

    sql_return("DELETE FROM public.token WHERE token_id='" + token + "'");
    
    // Create response JSON
    json::value response;

    // Send back the response
    request.reply(status_codes::OK, response);
}

void Token::handleGetRequest(http_request request) {
    // Parse JSON request
    auto json_value = request.extract_json().get();
    // Ensure the JSON value contains a "text" field.
    if (!json_value.has_field(U("clientId"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'clientId' field in JSON request."));
    	return;
    }
    
    auto clientId = json_value[U("clientId")].as_integer();
    std::string clientIdStr = std::to_string(clientId);
    auto token = sql_return("SELECT token_id FROM public.token WHERE client_id='" + clientIdStr + "' limit 1;");
    
    // Create response JSON
    json::value response;
    response[U("response")] = json::value::string(utility::conversions::to_string_t(token));

    // Send back the response
    request.reply(status_codes::OK, response);
}