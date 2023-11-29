// headers for http and json
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>
#include <pqxx/pqxx>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

// handles http get requests
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
            conn.disconnect();

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


// Todo: Placeholder function to mimic LLM behavior - revising user input to match a tone of voice.
std::string revise_text_conversation(const std::string& user_input) {
    // For now, we'll use a mock response.
    // For future iterations, this function will call LLM and generate a response.
    return "Howdy mate?";
}

int main() {
    
    // adds listener to localhost port	
    http_listener listener3(U("http://localhost:8080/login/user"));
    
    listener3.support(methods::GET, handle_request);
    
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
        std::string query = "SELECT * FROM users WHERE email='"+email+"'";
        // pqxx::result ret = sql_return("SELECT * FROM users WHERE email='js4777@example.com'");
        pqxx::result ret = sql_return(query);
        // Create response JSON
        json::value response_data;
        json::value userinfo;

        userinfo[U("id")] = json::value::string(utility::conversions::to_string_t(ret.begin()[0].c_str()));
        userinfo[U("first_name")] = json::value::string(utility::conversions::to_string_t(ret.begin()[1].c_str()));
        userinfo[U("last_name")] = json::value::string(utility::conversions::to_string_t(ret.begin()[2].c_str()));
        userinfo[U("email")] = json::value::string(utility::conversions::to_string_t(ret.begin()[3].c_str()));
        userinfo[U("password")] = json::value::string(utility::conversions::to_string_t(ret.begin()[4].c_str()));

        std::string psw = user_password;
        if(ret.begin()[4].c_str() == psw){
            response_data[U("response")] = json::value::string(utility::conversions::to_string_t("Login Sucess!"));
            response_data[U("userinfo")] = userinfo;
        } else {
            response_data[U("response")] = json::value::string(utility::conversions::to_string_t("Wrong email or password"));
        }

        // Send reply
        request.reply(status_codes::OK, response_data);
    });

    try {

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
    return 0;
}

