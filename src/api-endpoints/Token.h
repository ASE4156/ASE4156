#ifndef TOKEN_H 
#define TOKEN_H 

using namespace web;

class Token {
public:
    Token();
    void handleCreationRequest(web::http::http_request request);
    void handleDeletionRequest(web::http::http_request request);
    void handleGetRequest(web::http::http_request request);
    void handleValidateRequest(web::http::http_request request);
    void handleGetClientRequest(web::http::http_request request);
};

#endif // TOKEN_H 