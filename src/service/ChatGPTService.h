#ifndef CHATGPTSERVICE_H
#define CHATGPTSERVICE_H

#include <string>
#include <cpprest/http_client.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class ChatGPTService {
public:
    ChatGPTService(const std::string& apiKey);
    
    web::http::client::http_client setup_openai_client();
    web::json::value call_chatgpt_api_completion(const std::string& user_input, const std::string& prompt);
    web::json::value call_chatgpt_revise_conversation(const std::string& user_input);

private:
    std::string apiKey;
};

#endif // CHATGPTSERVICE_H