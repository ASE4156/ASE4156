#include "Prompt.h"
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <pqxx/pqxx>
#include "db.hpp"


using namespace web::http;


Prompt::Prompt() {}

void Prompt::handlePostRequest(http_request request) { 
    // Parse JSON request
    auto json_value = request.extract_json().get();
    // Ensure the JSON value contains a "text" field.
    if (!json_value.has_field(U("prompt_id"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'prompt_id' field in JSON request."));
    	return;
    }
    auto prompt_id = json_value[U("prompt_id")].as_integer();

    if (!json_value.has_field(U("prompt_name"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'prompt_name' field in JSON request."));
    	return;
    }
    auto prompt_name = json_value[U("prompt_name")].as_string();

    if (!json_value.has_field(U("prompt_description"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'prompt_description' field in JSON request."));
    	return;
    }
    auto prompt_description = json_value[U("prompt_description")].as_string();


    if (!json_value.has_field(U("prompt_content"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'prompt_content' field in JSON request."));
    	return;
    }
    auto prompt_content = json_value[U("prompt_content")].as_string();

    if (!json_value.has_field(U("client_id"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'client_id' field in JSON request."));
    	return;
    }
    int client_id = json_value[U("client_id")].as_integer();

    // Create response JSON
    json::value response;
    response[U("response")] = json::value::string("Insert Successful!");

    std::string query1 = R"(
        INSERT INTO prompt (prompt_id, prompt_name, prompt_description, prompt_content, client_id)
        VALUES 
            (%d, '%s', '%s', '%s', %d)
    )";

    char buffer[512]; // Choose an appropriate buffer size
    std::sprintf(buffer, query1.c_str(), prompt_id, prompt_name.c_str(), prompt_description.c_str(), prompt_content.c_str(), client_id);
    std::string finalQuery = buffer;
    // Send back the response
    std::string ret = sql_return(finalQuery);
    request.reply(status_codes::OK, response);
}