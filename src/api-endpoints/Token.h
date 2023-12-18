#ifndef TOKEN_H 
#define TOKEN_H 

using namespace web;

class Token {
public:
    Token();
    web::http::http_response handleCreationRequest(web::http::http_request request);
    web::http::http_response handleDeletionRequest(web::http::http_request request);
    web::http::http_response handleGetRequest(web::http::http_request request);
    web::http::http_response handleValidateRequest(web::http::http_request request);
    web::http::http_response handleGetClientRequest(web::http::http_request request);
};

#endif // TOKEN_H 