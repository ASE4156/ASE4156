#pragma once

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <string>

// namespace httpclient {

// Set up a client to the OpenAI API
web::http::client::http_client setup_openai_client();

// Make a call to the OpenAI ChatGPT API
web::json::value call_chatgpt_api_completion(const std::string& user_input);

web::json::value call_chatgpt_revise_conversation(const std::string& user_input);

// } // namespace httpclient
