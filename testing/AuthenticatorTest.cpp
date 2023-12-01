#include "catch.hpp"
#include <fstream> 
#include "Authenticator.h"
#include "db.hpp"

TEST_CASE("Authenticator Function Check") {
    Authenticator authenticator;
    std::string token = authenticator.generateToken();

    SECTION("VALIDATE TOKEN") {
        std::string dbToken = sql_return("SELECT token_id FROM public.token LIMIT 1;"); 
        REQUIRE(authenticator.validateToken(dbToken));
    }
 
    SECTION("Token Generation") {
        REQUIRE(token.length() > 0);
        REQUIRE(!(token == authenticator.generateToken()));
    }
    
}

