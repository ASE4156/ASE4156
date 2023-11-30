#include "catch.hpp"
#include "config/AppConfig.h"
#include <fstream> 

TEST_CASE("Retrieve API Key Successfully") {
    // Setup: Create a mock .env.test file with the expected content
    std::ofstream envFile(".env.test");
    envFile << "OPENAI_API_KEY=TestApiKey1234";
    envFile.close();

    // Test
    std::string apiKey = get_openai_api_key(".env.test");
    REQUIRE(apiKey == "TestApiKey1234");

    // Cleanup
    std::remove(".env.test");
}

TEST_CASE("API Key Not Found") {
    // Setup: Create a mock .env.test file without the API key
    std::ofstream envFile(".env.test");
    envFile << "SOME_OTHER_ENV_VAR=Value";
    envFile.close();

    // Test
    std::string apiKey = get_openai_api_key(".env.test");
    REQUIRE(apiKey.empty());

    // Cleanup
    std::remove(".env");
}

TEST_CASE("Missing .env File") {
    // Ensure no .env file exists

    // Test
    std::string apiKey = get_openai_api_key(".env.test");
    REQUIRE(apiKey.empty());
}