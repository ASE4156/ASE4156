#include "Authenticator.h"
#include "db.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>

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

std::string Authenticator::generateToken() {
    boost::uuids::random_generator generator;
    boost::uuids::uuid uuid = generator();
    return to_string(uuid);
}