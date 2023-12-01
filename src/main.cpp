// headers for http and json
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>
#include <pqxx/pqxx>
#include "config/AppConfig.h"
#include "service/ChatGPTService.h"
#include "service/Authenticator.h"
#include "api-endpoints/Conversation.h"
#include "api-endpoints/Token.h"
#include "api-endpoints/Prompt.h"
#include "api-endpoints/User.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

int main() {
    //Authenticator authenticator;
    //bool result = authenticator.validateToken("1234asdfasdf");
    //std::cout << result << std::endl;
    //std::cout << authenticator.generateToken() << std::endl;

    // Initialize services
    ChatGPTService chatGptService(get_openai_api_key());

    // Initialize Endpoint classes
    Conversation conversation(chatGptService);
    Token token_endpoint;
    Prompt prompt;
    User user;

    // Listener for the conversation endpoints
    http_listener conversation_listener(U("http://localhost:8080/llm/text/conversation"));
    conversation_listener.support(methods::POST, [&conversation](http_request request) {
        conversation.handleCompletionRequest(request);
    });

    http_listener token_creation_listener(U("http://localhost:8080/token/creation"));
    token_creation_listener.support(methods::GET, [&token_endpoint](http_request request) {
        token_endpoint.handleCreationRequest(request);
    });

    http_listener token_deletion_listener(U("http://localhost:8080/token/delete"));
    token_deletion_listener.support(methods::POST, [&token_endpoint](http_request request) {
        token_endpoint.handleDeletionRequest(request);
    });

    http_listener token_get_listener(U("http://localhost:8080/token/get"));
    token_get_listener.support(methods::GET, [&token_endpoint](http_request request) {
        token_endpoint.handleGetRequest(request);
    });

    http_listener user_creation_listener(U("http://localhost:8080/user/creation"));
    user_creation_listener.support(methods::GET, [&user](http_request request) {
        user.handleCreationRequest(request);
    });

    http_listener token_validate_listener(U("http://localhost:8080/token/validate"));
    token_validate_listener.support(methods::GET, [&token_endpoint](http_request request) {
        token_endpoint.handleValidateRequest(request);
    });

    http_listener token_get_client_listener(U("http://localhost:8080/token/getClient"));
    token_get_client_listener.support(methods::GET, [&token_endpoint](http_request request) {
        token_endpoint.handleGetClientRequest(request);
    });

    // Listener for the conversation endpoints
    http_listener prompt_listener(U("http://localhost:8080/prompt"));
    prompt_listener.support(methods::POST, [&prompt](http_request request) {
        prompt.handlePostRequest(request);
    });
    prompt_listener.support(methods::PUT, [&prompt](http_request request) {
        prompt.handlePutRequest(request);
    });
    prompt_listener.support(methods::DEL, [&prompt](http_request request) {
        prompt.handleDeleteRequest(request);
    });
    prompt_listener.support(methods::GET, [&prompt](http_request request) {
        prompt.handleGetRequest(request);
    });

    http_listener prompt_id_listener(U("http://localhost:8080/prompt/client_id"));
    prompt_id_listener.support(methods::GET, [&prompt](http_request request) {
        prompt.handleGetClientRequest(request);
    });

    try {
        conversation_listener.open().wait();
        token_creation_listener.open().wait();
        token_deletion_listener.open().wait();
        token_get_listener.open().wait();
        user_creation_listener.open().wait();
        token_validate_listener.open().wait();
        token_get_client_listener.open().wait();
        prompt_listener.open().wait();
        prompt_id_listener.open().wait();

            while (true);
    }
    catch (std::exception const & e) {
        std::wcerr<< e.what() << std::endl;
    }
    catch (const http_exception& e) {
        std::wcerr << e.what() << std::endl;
    }
}

