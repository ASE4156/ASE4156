#include "catch.hpp"
#include "service/ChatGPTService.h"
#include "config/AppConfig.h"
#include <cpprest/http_client.h>

TEST_CASE("ChatGPTService Basic Functionality Test") {
    ChatGPTService chatGPTService(get_openai_api_key());

    SECTION("Setup OpenAI Client") {
        auto client = chatGPTService.setup_openai_client();
        REQUIRE(client.base_uri().to_string() == U("https://api.openai.com"));
    }
}

TEST_CASE("ChatGPTService Completion API Integration Test") {
    /*
    Make an actual HTTP request to the OpenAI API and test successful interaction.
    */
    std::string userInput = "Hello, how are you?";
    std::string prompt = "You are an omniscient language expert who can respond to all types of requests.";
    ChatGPTService chatGPTService(get_openai_api_key());

    SECTION("Invalid API Key - Failure") {
	std::string invalidApiKey = "invalid_api_key";
	ChatGPTService invalidChatGPTService(invalidApiKey);
        auto responseJson = invalidChatGPTService.call_chatgpt_api_completion(userInput, prompt);

	REQUIRE(responseJson.has_field(U("error")));
    }

    SECTION("Invalid Request Data - failure") {
	std::string emptyInput = ""; // users should not be sending empty messages.

	REQUIRE_THROWS_AS(chatGPTService.call_chatgpt_api_completion(emptyInput, prompt), std::invalid_argument);
    }

    SECTION("Valid Key and Data - Success") {
        auto responseJson = chatGPTService.call_chatgpt_api_completion(userInput, prompt);

        // Validate the structure of the response
        REQUIRE(responseJson.is_object());
        REQUIRE(responseJson.has_field(U("response")));
        REQUIRE(responseJson[U("response")].is_string());
    }

}