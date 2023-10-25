#include <cpprest/http_client.h>
#include "openai.hpp"

using namespace web;
using namespace web::http;
using namespace web::http::client;

int main() {
    http_client client(U("http://localhost:8080/"));

    client.request(methods::GET).then([](http_response response) {
        std::wcout << U("Received response status code: ") << response.status_code() << std::endl;
        return response.extract_json();
    }).then([](json::value jsonResp) {
        std::cout << U("Received response data: ") << jsonResp.serialize() << std::endl;
    }).wait();
}

