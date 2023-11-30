#include "MockChatGPTService.h"
#include <cpprest/json.h>  
#include <stdexcept>    

MockChatGPTService::MockChatGPTService() {}

web::json::value MockChatGPTService::call_chatgpt_api_completion(const std::string& user_input, const std::string& prompt) {
    if (user_input.empty()) {
        throw std::invalid_argument("User input cannot be empty");
    }

    // Simulate a JSON response of the actual ChatGPT API
    web::json::value fakeResponse = web::json::value::object();

    // Construct a response with the following format:
    // https://platform.openai.com/docs/guides/text-generation/chat-completions-api
    web::json::value message = web::json::value::object();
    message[U("content")] = web::json::value::string(U("The 2020 World Series was played in Texas at Globe Life Field in Arlington."));
    message[U("role")] = web::json::value::string(U("assistant"));

    web::json::value choices = web::json::value::array(1); 
    choices[0][U("finish_reason")] = web::json::value::string(U("stop"));
    choices[0][U("index")] = web::json::value::number(0);
    choices[0][U("message")] = message;

    fakeResponse[U("choices")] = choices;

    return fakeResponse;
}