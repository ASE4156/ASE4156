#include "ChatGPTService.h"
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <string>
#include <iostream>
#include <fstream>

ChatGPTService::ChatGPTService(const std::string& key) : apiKey(key) {}

std::string MODEL = "gpt-3.5-turbo";

http_client ChatGPTService::setup_openai_client() {
    return http_client(U("https://api.openai.com"));
}

web::json::value ChatGPTService::call_chatgpt_api_completion(const std::string& user_input, const std::string& prompt) {
    if (user_input.empty()) {
	throw std::invalid_argument("User input cannot be empty");
    }

    std::cout << "Debug message: Value of prompt is " << prompt << std::endl;

    //std::cout << "call_chatgpt_api_completion has been called from http_client_utill.cpp." << std::endl;
    http_client client = setup_openai_client();
    http_request request(methods::POST);

    request.headers().set_content_type(U("application/json"));
    request.headers().add(U("Authorization"), U("Bearer " + apiKey));

    web::json::value payload = web::json::value::object();
    // corresponds to the required 'messages' payload parameter
    web::json::value messages = web::json::value::array();
    web::json::value system_message = web::json::value::object();
    web::json::value user_message = web::json::value::object();

    system_message[U("role")] = web::json::value::string(U("system"));
    system_message[U("content")] = web::json::value::string(U(prompt));

    user_message[U("role")] = web::json::value::string(U("user"));
    user_message[U("content")] = web::json::value::string(U(user_input));

    messages[0] = system_message;
    messages[1] = user_message;

    payload[U("messages")] = messages;
    payload[U("model")] = web::json::value::string(U(MODEL)); 
    request.set_body(payload);
    request.set_request_uri("/v1/chat/completions");
    
    //std::cout << "request pre-processing completed." << std::endl;
    //std::cout << payload.to_string() << std::endl;

    std::cout << "did we get here?" << std::endl;

    web::json::value response_json;
    client.request(request).then([&](http_response response) {
	if (response.status_code() == status_codes::OK) {
		//std::cout << "request returned and is okay." << std::endl;
		response_json = response.extract_json().get();
	} else {
	  //std::cout << "request returned but not okay." << std::endl;
		response_json = response.extract_json().get();
		//std::cout << response_json.to_string() << std::endl;
	}
    }).wait();
    return response_json;
}

// web::json::value ChatGPTService::call_chatgpt_revise_conversation(const std::string& user_input) {
    
//     //std::cout << "call_chatgpt_revise_conversation has been called from http_client_utill.cpp." << std::endl;
//     http_client client = setup_openai_client();
//     http_request request(methods::POST);

//     request.headers().set_content_type(U("application/json"));
//     request.headers().add(U("Authorization"), U("Bearer " + apiKey));

//     web::json::value payload = web::json::value::object();
//     // corresponds to the required 'messages' payload parameter
//     web::json::value messages = web::json::value::array();
//     web::json::value system_message = web::json::value::object();
//     web::json::value user_message = web::json::value::object();

//     system_message[U("role")] = web::json::value::string(U("system"));
//     system_message[U("content")] = web::json::value::string(U("You will revise every conversation that happens hereon and return the fluently revised version of the given prompt."));

//     user_message[U("role")] = web::json::value::string(U("user"));
//     user_message[U("content")] = web::json::value::string(U(user_input));

//     messages[0] = system_message;
//     messages[1] = user_message;

//     payload[U("messages")] = messages;
//     payload[U("model")] = web::json::value::string(U(MODEL)); 
//     request.set_body(payload);
//     request.set_request_uri("/v1/chat/completions");
    
//     //std::cout << "request pre-processing completed." << std::endl;
//     //std::cout << payload.to_string() << std::endl;

//     web::json::value response_json;
//     client.request(request).then([&](http_response response) {
// 	if (response.status_code() == status_codes::OK) {
// 		//std::cout << "request returned and is okay." << std::endl;
// 		response_json = response.extract_json().get();
// 	} else {
// 	  //std::cout << "request returned but not okay." << std::endl;
// 		response_json = response.extract_json().get();
// 		//std::cout << response_json.to_string() << std::endl;
// 	}
//     }).wait();
//     return response_json;
// }