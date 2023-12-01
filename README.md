# ASE4156

# Package Installation  
`brew install cpprestsdk`

`brew install openssl`

`brew install pkg-config, libpqxx` 
(if not installing pkg-config, do `export PKG_CONFIG_PATH="/opt/homebrew/opt/libpqxx/lib/pkgconfig"`)

`brew install boost`

# Compiling  

Navigate to repo's root directory, then  
1. `cmake .`
2. `make`
3. Then `./{executable_name}`

During development, make sure to clear make with 
```rm -rf CMakeFiles cmake_install.cmake CMakeCache.txt Makefile```
before running cmake again

# Endpoints

## LLM Text Conversation
- `POST /llm/text/conversation`
  - Description:
    - Calls prompted LLM conversation using saved prompts and OpenAI API
  - Request Body:
    - text: string 
    - token: string 
    - prompt_id: integer
  - Response:
    - JSON object with AI response
  - Response Codes:
    - 200: Success
    - 400: Missing or invalid 'text' or 'token' field, or invalid token in JSON request
    - 401: Unauthorized access due to invalid token

## Token 

- `GET /token/creation`
  - Description:
    - Creates a new token for client ID
  - Request Body:
    - clientId: integer 
  - Response:
    - JSON object containing the generated token.
  - Response Codes:
    - 200: Success
    - 400: Missing or invalid 'clientId' field in JSON request

- `POST /token/delete`
  - Description:
    - Deletes given token
  - Request Body:
    - token: string (The token to be deleted)
  - Response Codes:
    - 200: Success
    - 400: Missing or invalid 'token' field in JSON request or invalid token

- `GET /token/get`
  - Description:
    - Retrieves the token associated with a given client ID
  - Request Body:
    - clientId: integer 
  - Response:
    - JSON object containing the latest token for the client
  - Response Codes:
    - 200: Success
    - 400: Missing or invalid 'clientId' field in JSON request

- `GET /token/validate`
  - Description:
    - Validates the given token.
  - Request Body:
    - token: string (The token to be validated)
  - Response:
    - JSON object indicating whether the token is valid
  - Response Codes:
    - 200: Success
    - 400: Missing or invalid 'token' field in JSON request

- `GET /token/getClient`
  - Description:
    - Retrieves id of client associated with a given token
  - Request Body:
    - token: string
  - Response:
    - JSON object containing the client's ID and name
  - Response Codes:
    - 200: Success
    - 400: Missing or invalid 'token' field in JSON request

## User 

- `POST /user/creation`
  - Description:
    - Creates a new user with details in parameter 
  - Request Body:
    - clientName: string 
    - clientEmail: string 
    - clientPassword: string
  - Response:
    - JSON object containing the newly created client's ID.
  - Response Codes:
    - 200: Success

## Prompt

- `POST /prompt`
  - Description:
    - Creates a new prompt with the specified details.
  - Request Body:
    - token: string 
    - prompt_name: string 
    - prompt_description: string 
    - prompt_content: string 
    - client_id: integer 
  - Response Codes:
    - 200: Success
    - 400: Missing or invalid fields in JSON request

- `PUT /prompt`
  - Description:
    - Updates an existing prompt based on the provided prompt ID.
  - Request Body:
    - token: string 
    - prompt_id: integer 
    - prompt_name: string 
    - prompt_description: string 
    - prompt_content: string 
    - client_id: integer 
  - Response Codes:
    - 200: Success
    - 400: Missing or invalid fields in JSON request

- `DELETE /prompt`
  - Description:
    - Deletes an existing prompt based on the provided prompt ID.
  - Request Body:
    - token: string 
    - prompt_id: integer
  - Response Codes:
    - 200: Success
    - 400: Missing or invalid fields in JSON request

- `GET /prompt`
  - Description:
    - Retrieves information about an existing prompt based on the provided prompt ID.
  - Request Body:
    - token: string
    - prompt_id: integer 
  - Response Codes:
    - 200: Success
    - 400: Missing or invalid fields in JSON request

- `GET /prompt/client_id`
  - Description:
    - Retrieves information about prompts and client details based on the provided client ID.
  - Request Body:
    - token: string 
    - client_id: integer
  - Response Codes:
    - 200: Success
    - 400: Missing or invalid fields in JSON request

# Client App

https://github.com/ASE4156/client-app

# Testing

## unit-test

After compiling, at repo's root directory:
1. run ```./unit_test```
2. or run ```./unit_test -s``` to show test details.
3. or run ```./unit_test -s > testing/unit_test_report.txt``` to save to report

# Style-Checker

## cpplint

Required package:
```python or python3```

In the ```/styler_checker``` directory
1. run ```python3 cpplint.py [file]```

```/styler_checker/sc_command.sh``` check all cpp file in text and saved to report directory
1. run ```chmod +x sc_command.sh``` 
1. run ```./sc_command.sh```

## clang-format

Required package:
```sudo apt install clang-format```

run ```clang-format xxxx.cpp```

# 3rd Party API - OpenAI ChatGPT API   
https://platform.openai.com/api-keys

You must create your own OpenAI API token and put it in .env to utilize OpenAI's API.

The first line of the .env file must be as follows:  
`OPENAI_API_KEY=Your-key-here-without-quotations`