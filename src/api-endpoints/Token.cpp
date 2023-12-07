#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include "Token.h"
#include "Authenticator.h"
#include "db.hpp"

using namespace web::http;

Token::Token() {}

web::http::http_response Token::handleCreationRequest(http_request request) { 
    // Parse JSON request
    web::http::http_response endpointResponse;

    auto json_value = request.extract_json().get();
    // Ensure the JSON value contains a "text" field.
    if (!json_value.has_field(U("clientId"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'clientId' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
        endpointResponse.set_body(U("Missing or invalid 'clientId' field in JSON request."));
        return endpointResponse;;
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
    endpointResponse.set_status_code(web::http::status_codes::OK);
    endpointResponse.set_body(response);
    return endpointResponse;
}

web::http::http_response Token::handleDeletionRequest(http_request request) {
    // Parse JSON request
    web::http::http_response endpointResponse;

    auto json_value = request.extract_json().get();
    // Ensure the JSON value contains a "text" field.
    if (!json_value.has_field(U("token")) || !json_value[U("token")].is_string()) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'token' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
        endpointResponse.set_body(U("Missing or invalid 'token' field in JSON request."));
        return endpointResponse;;
    }

    auto token = json_value[U("token")].as_string();

    Authenticator authenticator;
    if (!authenticator.validateToken(token)) {
    	request.reply(status_codes::BadRequest, U("Invalid 'token' given."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
        endpointResponse.set_body(U("Invalid 'token' given."));
        return endpointResponse;;
    }

    sql_return("DELETE FROM public.token WHERE token_id='" + token + "'");
    
    // Create response JSON
    json::value response;

    // Send back the response
    request.reply(status_codes::OK, response);
    endpointResponse.set_status_code(web::http::status_codes::OK);
    endpointResponse.set_body(response);
    return endpointResponse;
}

web::http::http_response Token::handleGetRequest(http_request request) {
    // Parse JSON request
    web::http::http_response endpointResponse;

    auto json_value = request.extract_json().get();
    // Ensure the JSON value contains a "text" field.
    if (!json_value.has_field(U("clientId"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'clientId' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
        endpointResponse.set_body(U("Missing or invalid 'clientId' field in JSON request."));
        return endpointResponse;;
    }
    
    auto clientId = json_value[U("clientId")].as_integer();
    std::string clientIdStr = std::to_string(clientId);
    auto token = sql_return("SELECT token_id FROM public.token WHERE client_id='" + clientIdStr + "'ORDER BY created_at DESC LIMIT 1;");
    
    // Create response JSON
    json::value response;
    response[U("response")] = json::value::string(utility::conversions::to_string_t(token));

    // Send back the response
    request.reply(status_codes::OK, response);
    endpointResponse.set_status_code(web::http::status_codes::OK);
    endpointResponse.set_body(response);
    return endpointResponse;
}

web::http::http_response Token::handleValidateRequest(http_request request) {
    // Parse JSON request
    web::http::http_response endpointResponse;

    auto json_value = request.extract_json().get();
    // Ensure the JSON value contains a "text" field.
    if (!json_value.has_field(U("token"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'token' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
        endpointResponse.set_body(U("Missing or invalid 'token' field in JSON request."));
        return endpointResponse;;
    } 
    auto token = json_value[U("token")].as_string();
    
    Authenticator authenticator;
    bool validated = authenticator.validateToken(token);
    
    // Create response JSON
    json::value response;
    response[U("validated")] = json::value::boolean(validated);

    // Send back the response
    request.reply(status_codes::OK, response);
    endpointResponse.set_status_code(web::http::status_codes::OK);
    endpointResponse.set_body(response);
    return endpointResponse;
}

web::http::http_response Token::handleGetClientRequest(http_request request) {
    // Parse JSON request
    web::http::http_response endpointResponse;

    auto json_value = request.extract_json().get();
    // std::cout<<json_value<<std::endl;
    // Ensure the JSON value contains a "text" field.
    if (!json_value.has_field(U("token"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'token' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
        endpointResponse.set_body(U("Missing or invalid 'token' field in JSON request."));
        return endpointResponse;;
    } 
    auto token = json_value[U("token")].as_string(); 
    
    std::string clientIdStr = sql_return("SELECT client_id FROM public.token WHERE token_id='" + token + "';");
    int clientId = std::stoi(clientIdStr);
    std::string clientNameStr = sql_return("SELECT client_name FROM public.client WHERE client_id='" + clientIdStr + "';");

    // Create response JSON
    json::value response;
    response[U("clientId")] = json::value::number(clientId);
    response[U("clientName")] = json::value::string(clientNameStr);

    // Send back the response
    request.reply(status_codes::OK, response);
    endpointResponse.set_status_code(web::http::status_codes::OK);
    endpointResponse.set_body(response);
    return endpointResponse;
}