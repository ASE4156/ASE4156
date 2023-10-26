// headers for http and json
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include "http_client_util.hpp"
#include <iostream>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

// handles http get requests
void handle_request(http_request request) {
    json::value resp;
    resp[U("message")] = json::value::string(U("Hello, World!"));
    request.reply(status_codes::OK, resp);
}	

// Todo: Placeholder function to mimic LLM behavior - generating response to user input.
std::string respond_to_text_conversation(const std::string& user_input) {
    // Make the request
    auto response_json = call_chatgpt_api_completion(user_input);

    // Response format reference: 
    // https://platform.openai.com/docs/api-reference/chat/create
    return response_json[U("choices")][0][U("message")][U("content")].as_string();

    // For now, we'll use a mock response.
    // For future iterations, this function will call LLM and generate a response.
    // return "Thanks for talking to me. I was feeling a bit lonely.";
}

// Todo: Placeholder function to mimic LLM behavior - revising user input to match a tone of voice.
std::string revise_text_conversation(const std::string& user_input) {

    // Make request to revise conversation
    auto response_json = call_chatgpt_revise_conversation(user_input);

    // Response format reference: 
    // https://platform.openai.com/docs/api-reference/chat/create
    return response_json[U("choices")][0][U("message")][U("content")].as_string();
}

int main() {
    // adds listener to localhost port	
    http_listener listener(U("http://localhost:8080/llm/text/conversation"));
    
    listener.support(methods::GET, handle_request);
    
    // POST respond_to_text_conversation endpoint implementation
    listener.support(methods::POST, [](http_request request) {
        // Parse JSON request
        auto json_value = request.extract_json().get();

        // Ensure the JSON value contains a "text" field.
        if (!json_value.has_field(U("text")) || !json_value[U("text")].is_string()) {
            request.reply(status_codes::BadRequest, U("Missing or invalid 'text' field in JSON request."));
            return;
        }

        auto user_input = json_value[U("text")].as_string();
        auto processed_output = respond_to_text_conversation(utility::conversions::to_utf8string(user_input));

        // Create response JSON
        json::value response_data;
        response_data[U("response")] = json::value::string(utility::conversions::to_string_t(processed_output));

        // Send reply
        request.reply(status_codes::OK, response_data);
    });

    // adds listener to localhost port	
    http_listener listener2(U("http://localhost:8080/revise/user/input"));
    
    listener2.support(methods::GET, handle_request);
    
    // POST respond_to_text_conversation endpoint implementation
    listener2.support(methods::POST, [](http_request request) {
        // Parse JSON request
        auto json_value = request.extract_json().get();

        // Ensure the JSON value contains a "text" field.
        if (!json_value.has_field(U("text")) || !json_value[U("text")].is_string()) {
            request.reply(status_codes::BadRequest, U("Missing or invalid 'text' field in JSON request."));
            return;
        }

        auto user_input = json_value[U("text")].as_string();
        auto processed_output = revise_text_conversation(utility::conversions::to_utf8string(user_input));

        // Create response JSON
        json::value response_data;
        response_data[U("response")] = json::value::string(utility::conversions::to_string_t(processed_output));

        // Send reply
        request.reply(status_codes::OK, response_data);
    });

    try {
        listener
            .open()
            .then([&listener]() { std::wcout << U("Listening...") << std::endl; })
            .wait();

        listener2
            .open()
            .then([&listener]() { std::wcout << U("Listening...") << std::endl; })
                .wait();

            while (true);
    }
    catch (std::exception const & e) {
        std::wcerr<< e.what() << std::endl;
    }
    catch (const http_exception& e) {
        std::wcerr << e.what() << std::endl;
    }
}

