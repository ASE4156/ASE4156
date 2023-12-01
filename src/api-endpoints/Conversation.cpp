#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include "db.hpp"
#include "Conversation.h"
#include "Authenticator.h"

using namespace web::http;

Conversation::Conversation(IChatGPTService& service) : chatGptService(service) {}

web::http::http_response Conversation::handleCompletionRequest(http_request request) {
    web::http::http_response endpointResponse;

    // Parse JSON request
    auto json_value = request.extract_json().get();
    // Ensure the JSON value contains a "text" field.
    if (!json_value.has_field(U("text")) || !json_value[U("text")].is_string()) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'text' field in JSON request."));	
	    endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }
    else if (!json_value.has_field(U("token")) || !json_value[U("token")].is_string()) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'token' field in JSON request."));	
	    endpointResponse.set_status_code(web::http::status_codes::BadRequest);
	    return endpointResponse;
    }

    // we already checked above that token exists and is string
    std::string token = json_value[U("token")].as_string();
    Authenticator authenticator;
    if (!authenticator.validateToken(token)) {
        request.reply(status_codes::BadRequest, U("Invalid token given"));	
	    endpointResponse.set_status_code(web::http::status_codes::Unauthorized);
	    return endpointResponse; 
    }
    
    std::string prompt;
    auto userInput = json_value[U("text")].as_string();
    if (json_value.has_field(U("prompt_id"))) {
        int promptId = json_value[U("prompt_id")].as_integer();
        prompt = sql_return("SELECT prompt_content FROM public.prompt WHERE prompt_id='" + std::to_string(promptId) + "'");
    } else {
        // The field "prompt_id" does not exist, set a default prompt
        prompt = "You are a helpful language-expert chatbot.";
    }

    std::cout << prompt << std::endl;
    auto chatGptResponse = chatGptService.call_chatgpt_api_completion(
	utility::conversions::to_utf8string(userInput), prompt);
    std::cout << chatGptResponse << std::endl;
    auto processedResponse = chatGptResponse[U("choices")][0][U("message")][U("content")].as_string();

    // Create response JSON
    web::json::value response;
    response[U("response")] = web::json::value::string(utility::conversions::to_string_t(processedResponse));

    // Send back the response
    request.reply(status_codes::OK, response);
    endpointResponse.set_status_code(web::http::status_codes::OK);
    endpointResponse.set_body(response);
    return endpointResponse;
}