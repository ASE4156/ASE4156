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
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'text' field in JSON request."));
    	return;
    }
    auto clientId = json_value[U("clientId")].as_string();

    // Parse JSON request
    Authenticator authenticator;
    std::string uuid = authenticator.generateToken();

    // Create response JSON
    json::value response;
    response[U("response")] = json::value::string(utility::conversions::to_string_t(uuid));

    sql_return("INSERT INTO public.token(token_id, client_id) VALUES ('" + uuid + "', " + clientId + ")");

    // Send back the response
    request.reply(status_codes::OK, response);
}

void Token::handleDeletionRequest(http_request request) {
    // Parse JSON request
    auto json_value = request.extract_json().get();
    // Ensure the JSON value contains a "text" field.
    if (!json_value.has_field(U("token")) || !json_value[U("token")].is_string()) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'text' field in JSON request."));
    	return;
    }

    auto user_input = json_value[U("token")].as_string();
    sql_return("DELETE FROM public.token WHERE token_id='" + user_input + "'");
    
    // Create response JSON
    json::value response;

    // Send back the response
    request.reply(status_codes::OK, response);
}