# Postman Testing Guide

## Contents

1. `README.md` contains example input/output of each endpoint
2. 


## Example Input for Endpoints

**1. LLM Text Conversation (GET) -** `GET /llm/text/conversation`
   Returns a greeting message.
- Route: `http://localhost:8080/llm/text/conversation`
- Request: 
```
No request body required.
```

**2. LLM Text Conversation (POST) -** `POST /llm/text/conversation`
   Mimics LLM behavior by generating a response to user input.
- Route: `http://localhost:8080/llm/text/conversation`
- Request Body:
```json
{
    "text": "Howdy Howdy!"
}
```

**3. Revise User Input (GET) -** `GET /revise/user/input`
   Returns a greeting message.
- Route: `http://localhost:8080/revise/user/input`
- Request:
```
No request body required.
```

**4. Revise User Input (POST) -** `POST /revise/user/input`
   Mimics LLM behavior by revising user input to match a tone of voice.
- Route: `http://localhost:8080/revise/user/input`
- Request Body:
```json
{
    "text": "New Howdy Howdy!"
}
```
