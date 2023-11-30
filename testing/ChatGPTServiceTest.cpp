#include "catch.hpp"
#include "src/service/ChatGPTService.h"
#include "src/config/AppConfig.h"
#include <cpprest/http_client.h>

TEST_CASE("ChatGPTService Basic Functionality Test") {
    ChatGPTService chatGPTService(get_openai_api_key());

    SECTION("Setup OpenAI Client") {
        auto client = chatGPTService.setup_openai_client();
        REQUIRE(client.base_uri().to_string() == U("https://api.openai.com"));
    }
}

TEST_CASE("ChatGPTService Integration Test") {
    /*
    Make an actual HTTP request to the OpenAI API and test successful interaction.
    */

    SECTION("OpenAI Completion API Interaction") {
        std::string userInput = "Hello, how are you?";
        auto responseJson = chatGPTService.call_chatgpt_api_completion(userInput);

        // Validate the structure of the response
        REQUIRE(responseJson.is_object());
        REQUIRE(responseJson.has_field(U("response")));
        REQUIRE(responseJson[U("response")].is_string());
    }
}