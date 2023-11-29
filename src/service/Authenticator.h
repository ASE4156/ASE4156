#ifndef AUTHENTICATOR_H 
#define AUTHENTICATOR_H 

#include <string>

class Authenticator {
public:
    Authenticator();
    bool validateToken(const std::string& token);
};

#endif // AUTHENTICATOR_H 