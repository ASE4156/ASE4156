#include "Prompt.h"
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <pqxx/pqxx>
#include "db.hpp"


using namespace web::http;


Prompt::Prompt() {}

void Prompt::handlePostRequest(http_request request) { 
    auto json_value = request.extract_json().get();
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

    json::value response;
    response[U("response")] = json::value::string("Insert Successful!");

    std::string query1 = R"(
        INSERT INTO prompt (prompt_id, prompt_name, prompt_description, prompt_content, client_id)
        VALUES 
            (%d, '%s', '%s', '%s', %d)
    )";


    char buffer[512];
    std::string finalQuery;
    int result = std::snprintf(buffer, sizeof(buffer), query1.c_str(), prompt_id, prompt_name.c_str(), prompt_description.c_str(), prompt_content.c_str(), client_id);

    if (result >= 0 && result < sizeof(buffer)) {
        finalQuery = buffer;
    } else {
        finalQuery = "Error creating the query"; 
        return;
    }
    std::string ret = sql_return(finalQuery);
    request.reply(status_codes::OK, response);
}


void Prompt::handlePutRequest(http_request request) { 
    auto json_value = request.extract_json().get();
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

    json::value response;

    std::string query2 = R"(
        UPDATE prompt
        SET 
            prompt_name = '%s',
            prompt_description = '%s',
            prompt_content = '%s',
            client_id = %d
        WHERE prompt_id = %d;
    )";

    char buffer2[512];
    std::string finalQuery2;
    int result = std::snprintf(buffer2, sizeof(buffer2), query2.c_str(), prompt_name.c_str(), prompt_description.c_str(), prompt_content.c_str(), client_id, prompt_id);

    if (result >= 0 && result < sizeof(buffer2)) {
        finalQuery2 = buffer2;
    } else {
        finalQuery2 = "Error creating the query"; 
        return;
    }

    std::string ret = sql_return(finalQuery2);

    response[U("response")] = json::value::string("Update Sucessfully!");
    request.reply(status_codes::OK, response);
}


void Prompt::handleDeleteRequest(http_request request) {
    auto json_value = request.extract_json().get();
    if (!json_value.has_field(U("prompt_id"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'prompt_id' field in JSON request."));
    	return;
    }

    auto prompt_id = json_value[U("prompt_id")].as_integer();

    std::string query3 = R"(
        DELETE FROM prompt
        WHERE prompt_id = %d;
    )";

    char buffer3[512];
    std::string finalQuery3;
    int result = std::snprintf(buffer3, sizeof(buffer3), query3.c_str(),prompt_id);

    if (result >= 0 && result < sizeof(buffer3)) {
        finalQuery3 = buffer3;
    } else {
      
        finalQuery3 = "Error creating the query";
        return;
    }

    std::string ret = sql_return(finalQuery3);

    json::value response;
    response[U("response")] = json::value::string("Delete Sucessfully!");

    request.reply(status_codes::OK, response);
}


void Prompt::handleGetRequest(http_request request) {
    auto json_value = request.extract_json().get();
    if (!json_value.has_field(U("prompt_id"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'prompt_id' field in JSON request."));
    	return;
    }
    auto prompt_id = json_value[U("prompt_id")].as_integer();    
    pqxx::result prompt = sql_return_result("SELECT * FROM prompt WHERE prompt_id = "+std::to_string(prompt_id)+";");

    json::value prompt_info;
    std::vector<std::string> promptList = {
        "prompt_id", "prompt_name", "prompt_description", "prompt_content", "client_id"
    };

    int index = 0;
    for (pqxx::result::const_iterator row = prompt.begin(); row != prompt.end(); ++row) {
        json::value prompt_row;
        for (size_t col = 0; col < row.size(); ++col) {
            prompt_row[U(promptList[col])] = json::value::string(row[col].c_str());
        }
        prompt_info[index++] = prompt_row;
    }

    json::value response;
    response[U("response")] = prompt_info;

    request.reply(status_codes::OK, response);
}

void Prompt::handleGetClientRequest(http_request request) {
    auto json_value = request.extract_json().get();

    if (!json_value.has_field(U("client_id"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'client_id' field in JSON request."));
    	return;
    }
    auto client_id = json_value[U("client_id")].as_integer();    
    pqxx::result prompt = sql_return_result("SELECT * FROM prompt WHERE client_id = "+std::to_string(client_id)+";");

    json::value prompt_info;
    std::vector<std::string> promptList = {
        "prompt_id", "prompt_name", "prompt_description", "prompt_content", "client_id"
    };

    int index = 0;
    for (pqxx::result::const_iterator row = prompt.begin(); row != prompt.end(); ++row) {
        json::value prompt_row;
        for (size_t col = 0; col < row.size(); ++col) {
            prompt_row[U(promptList[col])] = json::value::string(row[col].c_str());
        }
        prompt_info[index++] = prompt_row;
    }

    json::value response;
    response[U("response")] = prompt_info;

    request.reply(status_codes::OK, response);
}