#ifndef CONVERSATION_H
#define CONVERSATION_H

class Conversation {

public:
    Conversation(ChatGPTService& service);
    void handleCompletionRequest(web::http::http_request request);

private:
    ChatGPTService& chatGptService;
};

#endif // CONVERSATION_H