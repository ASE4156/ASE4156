# ASE4156

# Package Installation  
`brew install cpprestsdk`

`brew install openssl`

# Compiling  

Navigate to repo's root directory, then  
1. `cmake .`
2. `make`
3. Then `./{executable_name}`

During development, make sure to clear make with 
```rm -rf CMakeFiles cmake_install.cmake CMakeCache.txt Makefile```
before running cmake again

# Endpoints

- `GET /llm/text/conversation`
  - Description:
    - Returns a greeting message.
  - Request Body:
    - None.
  - Response Codes:
    - 200: Success
  
- POST /llm/text/conversation
  - Description:
    - Mimics LLM behavior by generating a response to user input.
  - Request Body:
    - text: string (User's input text)
  - Response Codes:
    - 200: Success
    - 400: Missing or invalid 'text' field in JSON request
   
- GET /revise/user/input
  - Description:
    - Returns a greeting message.
  - Request Body:
    - None.
  - Response Codes:
    - 200: Success
- POST /revise/user/input
  - Description:
    - Mimics LLM behavior by revising user input to match a tone of voice.
  - Request Body:
    - text: string (User's input text)
  - Response Codes:
    - 200: Success
    - 400: Missing or invalid 'text' field in JSON request

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
















