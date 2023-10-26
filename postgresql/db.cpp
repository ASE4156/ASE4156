#include <iostream>
#include <pqxx/pqxx>

int sql_return(const std::string& query) {
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

            // Process the result
            for (pqxx::result::const_iterator row = result.begin(); row != result.end(); ++row) {
                std::cout << row[0].c_str() << std::endl;  // Assuming the first column is of type text
            }

            txn.commit();
            conn.disconnect();
        } else {
            std::cerr << "Failed to open database" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

int main(){

    return sql_return("SELECT password FROM users WHERE email='js4777@example.com'");
}