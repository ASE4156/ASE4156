#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include "User.h"
#include "db.hpp"

using namespace web::http;

User::User() {}

void User::handleCreationRequest(http_request request) { 
    // Parse JSON request
    auto json_value = request.extract_json().get();
    // Ensure the JSON value contains a "text" field.
    auto clientName = json_value[U("clientName")].as_string();
    auto clientEmail = json_value[U("clientEmail")].as_string();
    auto clientPassword = json_value[U("clientPassword")].as_string();

    sql_return("INSERT INTO public.client(client_name, client_email, client_password) \
                VALUES ('" + clientName + "', '" + clientEmail + "', '" + clientPassword + "')");
    
    std::string clientIdStr = sql_return("SELECT client_id FROM public.client \
                                WHERE client_name='" + clientName + "' AND client_email='" + clientEmail + "' AND client_password='" + clientPassword + "' \
                                ORDER BY created_at DESC LIMIT 1;");
    int clientId = std::stoi(clientIdStr);

    // Create response JSON
    json::value response;
    response[U("clientId")] = json::value::number(clientId);

    // Send back the response
    request.reply(status_codes::OK, response);
}