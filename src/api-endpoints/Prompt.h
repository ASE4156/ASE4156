#ifndef PROMPT_H 
#define PROMPT_H 

#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;

class Prompt {
public:
    Prompt();
    void handlePostRequest(web::http::http_request request);
    void handleGetClientRequest(web::http::http_request request);
    void handleGetRequest(web::http::http_request request);
    void handlePutRequest(web::http::http_request request);
    void handleDeleteRequest(web::http::http_request request);
};

#endif 