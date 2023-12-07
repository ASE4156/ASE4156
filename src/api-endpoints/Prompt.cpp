#include "Prompt.h"
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <pqxx/pqxx>
#include "db.hpp"
#include "Authenticator.h"
#include <cstring>
#include <iostream>


using namespace web::http;


Prompt::Prompt() {}

web::http::http_response Prompt::handlePostRequest(http_request request) { 
    auto json_value = request.extract_json().get();
    web::http::http_response endpointResponse;

    if (!json_value.has_field(U("token")) || !json_value[U("token")].is_string()) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'token' field in JSON request."));	
        endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }

    // we already checked above that token exists and is string
    std::string token = json_value[U("token")].as_string();
    Authenticator authenticator;
    if (!authenticator.validateToken(token)) {
        request.reply(status_codes::BadRequest, U("Invalid token given"));	
	    endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse; 
    }

    if (!json_value.has_field(U("prompt_name"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'prompt_name' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }
    auto prompt_name = json_value[U("prompt_name")].as_string();

    if (!json_value.has_field(U("prompt_description"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'prompt_description' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }
    auto prompt_description = json_value[U("prompt_description")].as_string();


    if (!json_value.has_field(U("prompt_content"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'prompt_content' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }
    auto prompt_content = json_value[U("prompt_content")].as_string();

    if (!json_value.has_field(U("client_id"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'client_id' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }
    int client_id = json_value[U("client_id")].as_integer();

    // find the biggest id in data base, and create id 1 bigger than that num
    pqxx::result max_id = sql_return_result("SELECT MAX(prompt_id) FROM prompt;");

    std::string str_value = max_id[0][0].as<std::string>();  
    int prompt_id = std::stoi(str_value);
    prompt_id+=1;  

    json::value response;

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
        endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }
    std::string ret = sql_return(finalQuery);

    // std::cout << ret.c_str() << std::endl;
    if (ret.empty()){
        response[U("response")] = json::value::string("Insert Successful prompt_id "+std::to_string(prompt_id)+"!");
        response[U("prompt_id")] = json::value::string(std::to_string(prompt_id));
        request.reply(status_codes::OK, response);
        endpointResponse.set_status_code(web::http::status_codes::OK);
        endpointResponse.set_body(response);
	    return endpointResponse;
    } else {
        response[U("response")] = json::value::string(ret.c_str());
        request.reply(status_codes::BadRequest, response);
        endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;

    }
}


web::http::http_response Prompt::handlePutRequest(http_request request) { 
    auto json_value = request.extract_json().get();
    web::http::http_response endpointResponse;

    if (!json_value.has_field(U("token")) || !json_value[U("token")].is_string()) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'token' field in JSON request."));	
	    endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }

    // we already checked above that token exists and is string
    std::string token = json_value[U("token")].as_string();
    Authenticator authenticator;
    if (!authenticator.validateToken(token)) {
        request.reply(status_codes::BadRequest, U("Invalid token given"));	
	    endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse; 
    }

    if (!json_value.has_field(U("prompt_id"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'prompt_id' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }
    auto prompt_id = json_value[U("prompt_id")].as_integer();

    if (!json_value.has_field(U("prompt_name"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'prompt_name' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }
    auto prompt_name = json_value[U("prompt_name")].as_string();

    if (!json_value.has_field(U("prompt_description"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'prompt_description' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }
    auto prompt_description = json_value[U("prompt_description")].as_string();

    if (!json_value.has_field(U("prompt_content"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'prompt_content' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }
    auto prompt_content = json_value[U("prompt_content")].as_string();

    if (!json_value.has_field(U("client_id"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'client_id' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
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
        endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }

    json::value prompt_info;
    std::vector<std::string> promptList = {
        "prompt_id", "prompt_name", "prompt_description", "prompt_content", "client_id"
    };

    int index = 0;
    pqxx::result prompt = sql_return_result("SELECT * FROM prompt WHERE prompt_id = "+std::to_string(prompt_id)+";");
    for (pqxx::result::const_iterator row = prompt.begin(); row != prompt.end(); ++row) {
        json::value prompt_row;
        for (size_t col = 0; col < row.size(); ++col) {
            prompt_row[U(promptList[col])] = json::value::string(row[col].c_str());
        }
        prompt_info[index++] = prompt_row;
    }
    
    if (!prompt_info.is_null()){
        std::string ret = sql_return(finalQuery2);
        response[U("response")] = json::value::string("Update Successful! prompt_id "+std::to_string(prompt_id)+"!");
        response[U("Previous info")] = prompt_info;
        request.reply(status_codes::OK, response);
        endpointResponse.set_status_code(web::http::status_codes::OK);
	    return endpointResponse;
    } else {
        response[U("response")] = json::value::string("prompt_id "+std::to_string(prompt_id)+" not exist!");
        request.reply(status_codes::BadRequest, response);
        endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }
    endpointResponse.set_status_code(web::http::status_codes::OK);
	return endpointResponse;

}


web::http::http_response Prompt::handleDeleteRequest(http_request request) {
    auto json_value = request.extract_json().get();
    web::http::http_response endpointResponse;

    if (!json_value.has_field(U("token")) || !json_value[U("token")].is_string()) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'token' field in JSON request."));	
	    endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }

    // we already checked above that token exists and is string
    std::string token = json_value[U("token")].as_string();
    Authenticator authenticator;
    if (!authenticator.validateToken(token)) {
        request.reply(status_codes::BadRequest, U("Invalid token given"));	
	    endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse; 
    }

    if (!json_value.has_field(U("prompt_id"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'prompt_id' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
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
        endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }

    json::value response;
    json::value prompt_info;
    std::vector<std::string> promptList = {
        "prompt_id", "prompt_name", "prompt_description", "prompt_content", "client_id"
    };

    int index = 0;
    pqxx::result prompt = sql_return_result("SELECT * FROM prompt WHERE prompt_id = "+std::to_string(prompt_id)+";");
    for (pqxx::result::const_iterator row = prompt.begin(); row != prompt.end(); ++row) {
        json::value prompt_row;
        for (size_t col = 0; col < row.size(); ++col) {
            prompt_row[U(promptList[col])] = json::value::string(row[col].c_str());
        }
        prompt_info[index++] = prompt_row;
    }
    
    if (!prompt_info.is_null()){
        std::string ret = sql_return(finalQuery3);
        response[U("response")] = json::value::string("Delete Successful! prompt_id "+std::to_string(prompt_id)+"!");
        response[U("deleted info")] = prompt_info;
        request.reply(status_codes::OK, response);
        endpointResponse.set_status_code(web::http::status_codes::OK);
	    return endpointResponse;
    } else {
        response[U("response")] = json::value::string("prompt_id "+std::to_string(prompt_id)+" not exist!");
        request.reply(status_codes::BadRequest, response);
        endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }
}


web::http::http_response Prompt::handleGetRequest(http_request request) {
    auto json_value = request.extract_json().get();
    web::http::http_response endpointResponse;

    if (!json_value.has_field(U("token")) || !json_value[U("token")].is_string()) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'token' field in JSON request."));	
        endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }

    // we already checked above that token exists and is string
    std::string token = json_value[U("token")].as_string();
    Authenticator authenticator;
    if (!authenticator.validateToken(token)) {
        request.reply(status_codes::BadRequest, U("Invalid token given"));	
        endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }

    if (!json_value.has_field(U("prompt_id"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'prompt_id' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
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
    if (!prompt_info.is_null()){
        response[U("response")] = prompt_info;
        request.reply(status_codes::OK, response);
        endpointResponse.set_status_code(web::http::status_codes::OK);
	    return endpointResponse;
    } else {
        response[U("response")] = json::value::string("prompt_id "+std::to_string(prompt_id)+" not exist!");
        request.reply(status_codes::BadRequest, response);
        endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }

}

web::http::http_response Prompt::handleGetClientRequest(http_request request) {
    auto json_value = request.extract_json().get();
    web::http::http_response endpointResponse;
    json::value response;

    if (!json_value.has_field(U("token")) || !json_value[U("token")].is_string()) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'token' field in JSON request."));	
        endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }

    // we already checked above that token exists and is string
    std::string token = json_value[U("token")].as_string();
    Authenticator authenticator;
    if (!authenticator.validateToken(token)) {
        request.reply(status_codes::BadRequest, U("Invalid token given"));	
        endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }

    if (!json_value.has_field(U("client_id"))) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'client_id' field in JSON request."));
    	endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }
    auto client_id = json_value[U("client_id")].as_integer();    
    pqxx::result client = sql_return_result("SELECT * FROM client WHERE client_id = "+std::to_string(client_id)+";");

    json::value client_info;
    std::vector<std::string> clientList = {
        "client_id", "client_name","client_email", "client_password", "created_at"
    };

    int index = 0;
    for (pqxx::result::const_iterator row = client.begin(); row != client.end(); ++row) {
        json::value prompt_row;
        for (size_t col = 0; col < row.size(); ++col) {
            prompt_row[U(clientList[col])] = json::value::string(row[col].c_str());
        }
        client_info[index++] = prompt_row;
    }

    if (client_info.is_null()){
        response[U("response")] = json::value::string("client_id "+std::to_string(client_id)+" not exist!");
        request.reply(status_codes::BadRequest, response);
        endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }

    pqxx::result prompt = sql_return_result("SELECT * FROM prompt WHERE client_id = "+std::to_string(client_id)+";");
    json::value prompt_info;
    std::vector<std::string> promptList = {
        "prompt_id", "prompt_name", "prompt_description", "prompt_content", "client_id"
    };

    index = 0;
    for (pqxx::result::const_iterator row = prompt.begin(); row != prompt.end(); ++row) {
        json::value prompt_row;
        for (size_t col = 0; col < row.size(); ++col) {
            prompt_row[U(promptList[col])] = json::value::string(row[col].c_str());
        }
        prompt_info[index++] = prompt_row;
    }


    response[U("response")] = prompt_info;
    response[U("client")] = client_info;

    request.reply(status_codes::OK, response);
    endpointResponse.set_status_code(web::http::status_codes::OK);
	return endpointResponse;
}