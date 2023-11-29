#ifndef AUTHENTICATOR_H 
#define AUTHENTICATOR_H 

#include <string>

class Authenticator {
public:
    Authenticator();
    bool validateToken(const std::string& token);
    std::string generateToken();
};

#endif // AUTHENTICATOR_H 