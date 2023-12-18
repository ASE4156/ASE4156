#ifndef USER_H 
#define USER_H 

#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;

class User {
public:
    User();
    web::http::http_response handleCreationRequest(web::http::http_request request);
};

#endif // USER_H 