#ifndef CONVERSATION_H
#define CONVERSATION_H

#include "service/ChatGPTService.h"

using namespace web;

class Conversation {

public:
    Conversation(ChatGPTService& service);
    void handleCompletionRequest(web::http::http_request request);

private:
    ChatGPTService& chatGptService;
};

#endif // CONVERSATION_H