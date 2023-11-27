#include "Conversation.h"
#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web::http;

Conversation::Conversation(ChatGPTService& service) : chatGptService(service) {}

void Conversation::handleCompletionRequest(http_request request) {
    // Parse JSON request
    auto json_value = request.extract_json().get();
    // Ensure the JSON value contains a "text" field.
    if (!json_value.has_field(U("text")) || !json_value[U("text")].is_string()) {
    	request.reply(status_codes::BadRequest, U("Missing or invalid 'text' field in JSON request."));
    	return;
    }

    auto user_input = json_value[U("text")].as_string();
    auto chatGptResponse = chatGptService.call_chatgpt_api_completion(
	utility::conversions::to_utf8string(user_input));
    auto processedResponse = chatGptResponse[U("choices")][0][U("message")][U("content")].as_string();

    // Create response JSON
    json::value response;
    response[U("response")] = json::value::string(utility::conversions::to_string_t(processedResponse));

    // Send back the response
    request.reply(status_codes::OK, response);
}