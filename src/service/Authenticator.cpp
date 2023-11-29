#include <iostream>
#include "Authenticator.h"
#include "db.hpp"

Authenticator::Authenticator() {}

bool Authenticator::validateToken(const std::string& token) {
    std::string query = "SELECT client_id FROM public.token WHERE token_id='" + token + "'";
    //std::cout << query << std::endl;
    std::string result = sql_return(query);
    if (result.length() == 0) {
        return 0;
    } else {
        return 1;
    }
}