#ifndef USER_H 
#define USER_H 

using namespace web;

class User {
public:
    User();
    void handleCreationRequest(web::http::http_request request);
};

#endif // USER_H 