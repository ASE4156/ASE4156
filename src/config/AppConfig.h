#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <string>

std::string get_openai_api_key(const std::string& envFileName=".env");

#endif // APPCONFIG_H