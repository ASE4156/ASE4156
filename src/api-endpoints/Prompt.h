#ifndef PROMPT_H 
#define PROMPT_H 

#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;

class Prompt {
public:
    Prompt();
    web::http::http_response handlePostRequest(web::http::http_request request);
    web::http::http_response handleGetClientRequest(web::http::http_request request);
    web::http::http_response handleGetRequest(web::http::http_request request);
    web::http::http_response handlePutRequest(web::http::http_request request);
    web::http::http_response handleDeleteRequest(web::http::http_request request);
};

#endif 