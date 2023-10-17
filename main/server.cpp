// headers for http and json
#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

// handles http get requests
void handle_request(http_request request) {
    json::value resp;
    resp[U("message")] = json::value::string(U("Hello, World!"));
    request.reply(status_codes::OK, resp);
}	

int main() {
    // adds listener to localhost port	
    http_listener listener(U("http://localhost:8080/"));
    listener.support(methods::GET, handle_request);

    try {
        listener
            .open()
            .then([&listener]() { std::wcout << U("Listening...") << std::endl; })
            .wait();

        while (true);
    }
    catch (std::exception const & e) {
        std::wcout << e.what() << std::endl;
    }
}

