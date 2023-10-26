#include "http_client_util.hpp"
#include "env_util.hpp"
#include <cpprest/json.h>
#include <string>
#include <iostream>

using namespace web;
using namespace web::http;
using namespace web::http::client;

// namespace httpclient {
std::string MODEL = "gpt-3.5-turbo";

http_client setup_openai_client() {
    return http_client(U("https://api.openai.com"));
}

web::json::value call_chatgpt_api_completion(const std::string& user_input) {
    
//     std::cout << "call_chatgpt_api_completion has been called from http_client_utill.cpp." << std::endl;
    http_client client = setup_openai_client();
    http_request request(methods::POST);

    request.headers().set_content_type(U("application/json"));
    request.headers().add(U("Authorization"), U("Bearer " + get_openai_api_key()));

    web::json::value payload = web::json::value::object();
    // corresponds to the required 'messages' payload parameter
    web::json::value messages = web::json::value::array();
    web::json::value system_message = web::json::value::object();
    web::json::value user_message = web::json::value::object();

    system_message[U("role")] = web::json::value::string(U("system"));
    system_message[U("content")] = web::json::value::string(U("You are a helpful language-expert chatbot."));

    user_message[U("role")] = web::json::value::string(U("user"));
    user_message[U("content")] = web::json::value::string(U(user_input));

    messages[0] = system_message;
    messages[1] = user_message;

    payload[U("messages")] = messages;
    payload[U("model")] = web::json::value::string(U(MODEL)); 
    request.set_body(payload);
    request.set_request_uri("/v1/chat/completions");
    
//     std::cout << "request pre-processing completed." << std::endl;
//     std::cout << payload.to_string() << std::endl;

    web::json::value response_json;
    client.request(request).then([&](http_response response) {
	if (response.status_code() == status_codes::OK) {
		// std::cout << "request returned and is okay." << std::endl;
		response_json = response.extract_json().get();
	} else {
	   	// std::cout << "request returned but not okay." << std::endl;
		response_json = response.extract_json().get();
		// std::cout << response_json.to_string() << std::endl;
	}
    }).wait();
    return response_json;
}

// } 