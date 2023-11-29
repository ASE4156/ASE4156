// headers for http and json
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>
#include <pqxx/pqxx>
#include "config/AppConfig.h"
#include "service/ChatGPTService.h"
#include "api-endpoints/Conversation.h"


using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

// handles http get requests
// Todo (Wei): remove this function once listener3 is refactored into the MVC pattern.
void handle_request(http_request request) {
    json::value resp;
    resp[U("message")] = json::value::string(U("Hello, World!"));
    request.reply(status_codes::OK, resp);
}	

pqxx::result sql_return(const std::string& query) {
    try {
        // Define your connection parameters
        std::string host = "ase4156.clyigb9dssrd.us-east-1.rds.amazonaws.com";
        std::string dbname = "postgres";
        std::string user = "dbuser";
        std::string password = "dbuserdbuser";
        const int sqlport = 5432;  // PostgreSQL default port

        // Construct the connection string
        std::string connection_string = "host=" + host +
                                      " dbname=" + dbname +
                                      " user=" + user +
                                      " password=" + password +
                                      " port=" + std::to_string(sqlport);

        pqxx::connection conn(connection_string);

        if (conn.is_open()) {
            std::cout << "Opened database successfully: " << conn.dbname() << std::endl;

            pqxx::work txn(conn);

            // Execute a query
            pqxx::result result = txn.exec(query);

            // // Process the result
            // for (pqxx::result::const_iterator row = result.begin(); row != result.end(); ++row) {
            //     std::cout << row[0].c_str() << std::endl;  // Assuming the first column is of type text
            // }

            txn.commit();
            conn.close();

            return result;
        } else {
            std::cerr << "Failed to open database" << std::endl;
            pqxx::result emptyresult;
            return emptyresult;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        pqxx::result emptyresult;
        return emptyresult;
    }

    pqxx::result emptyresult;
    return emptyresult;
}


int main() {
    // Initialize services
    ChatGPTService chatGptService(get_openai_api_key());

    // Initialize Endpoint classes
    Conversation conversation(chatGptService);

    // Listener for the conversation endpoints
    http_listener conversation_listener(U("http://localhost:8080/llm/text/conversation"));
    conversation_listener.support(methods::POST, [&conversation](http_request request) {
        conversation.handleCompletionRequest(request);
    });

    // adds listener to localhost port	
    http_listener listener3(U("http://localhost:8080/login/user"));
    
    listener3.support(methods::GET, handle_request);
    
    // Todo (Wei): need separation of the logic below from the main function.
    // POST respond_to_text_conversation endpoint implementation
    listener3.support(methods::POST, [](http_request request) {
        // Parse JSON request
        auto json_value = request.extract_json().get();

        // Ensure the JSON value contains a "email" field.
        if (!json_value.has_field(U("email")) || !json_value[U("email")].is_string()) {
            request.reply(status_codes::BadRequest, U("Missing or invalid 'email' field in JSON request."));
            return;
        }
        // Ensure the JSON value contains a "password" field.
        if (!json_value.has_field(U("password")) || !json_value[U("password")].is_string()) {
            request.reply(status_codes::BadRequest, U("Missing or invalid 'password' field in JSON request."));
            return;
        }

        auto user_email = json_value[U("email")].as_string();
        auto user_password = json_value[U("password")].as_string();
        std::string email = user_email;
        std::string query = "SELECT * FROM client ";
        // pqxx::result ret = sql_return("SELECT * FROM users WHERE email='js4777@example.com'");
        pqxx::result ret = sql_return(query);
        // Create response JSON
        json::value response_data;
        json::value userinfo;

        // Send reply
        request.reply(status_codes::OK, response_data);
    });

    try {
        conversation_listener.open().wait();

        listener3
            .open()
            .then([&listener3]() { std::wcout << U("Listening...") << std::endl; })
                .wait();

            while (true);
    }
    catch (std::exception const & e) {
        std::wcerr<< e.what() << std::endl;
    }
    catch (const http_exception& e) {
        std::wcerr << e.what() << std::endl;
    }
}

