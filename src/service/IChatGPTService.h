#ifndef ICHATGPTSERVICE_H
#define ICHATGPTSERVICE_H

#include <string>
#include <cpprest/json.h>

class IChatGPTService {
public:
    virtual ~IChatGPTService() = default;
    virtual web::json::value call_chatgpt_api_completion(const std::string& user_input, const std::string& prompt) = 0;
};

#endif // ICHATGPTSERVICE_H