#pragma once

#include <string>

// Reads machine-specific local enviornment variables from the .env file.
std::string get_openai_api_key();