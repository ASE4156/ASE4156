#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include "Token.h"
#include "Authenticator.h"

using namespace web::http;

Token::Token() {}

void Token::handleCreationRequest(http_request request) {
    // Parse JSON request
    Authenticator authenticator;
    std::string uuid = authenticator.generateToken();

    // Create response JSON
    json::value response;
    response[U("response")] = json::value::string(utility::conversions::to_string_t(uuid));

    // Send back the response
    request.reply(status_codes::OK, response);
}