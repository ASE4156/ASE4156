#ifndef CONVERSATION_H
#define CONVERSATION_H

#include "service/IChatGPTService.h"
#include <cpprest/http_client.h>

class Conversation {

public:
    Conversation(IChatGPTService& service);
    web::http::http_response handleCompletionRequest(web::http::http_request request);

private:
    IChatGPTService& chatGptService;
};

#endif // CONVERSATION_H