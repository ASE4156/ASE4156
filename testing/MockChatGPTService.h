#ifndef MOCKCHATGPTSERVICE_H
#define MOCKCHATGPTSERVICE_H

#include "service/IChatGPTService.h"

class MockChatGPTService : public IChatGPTService {
public:
    MockChatGPTService();
    web::json::value call_chatgpt_api_completion(const std::string& user_input, const std::string& prompt) override;
};

#endif // MOCKCHATGPTSERVICE_H