#include <iostream>
#include <pqxx/pqxx>
#include "db.hpp"

std::string sql_return(const std::string& query) {
    try {
        std::string host = "ase4156.clyigb9dssrd.us-east-1.rds.amazonaws.com";
        std::string dbname = "postgres";
        std::string user = "dbuser";
        std::string password = "dbuserdbuser";
        const int sqlport = 5432;  

        std::string connection_string = "host=" + host +
                                      " dbname=" + dbname +
                                      " user=" + user +
                                      " password=" + password +
                                      " port=" + std::to_string(sqlport);

        pqxx::connection conn(connection_string);

        if (conn.is_open()) {
            pqxx::work txn(conn);

            pqxx::result result = txn.exec(query);

            if (result.empty()) {
                txn.commit();
                conn.close();
                return "";
            }

            for (pqxx::result::const_iterator row = result.begin(); row != result.end(); ++row) {
            }

            txn.commit();
            conn.close();

            return result.begin()[0].c_str();
        } else {
            std::cerr << "Failed to open database" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return "1";
    }

    return "0";
}


pqxx::result sql_return_result(const std::string& query) {
    pqxx::result empty;
    try {
        std::string host = "ase4156.clyigb9dssrd.us-east-1.rds.amazonaws.com";
        std::string dbname = "postgres";
        std::string user = "dbuser";
        std::string password = "dbuserdbuser";
        const int sqlport = 5432;  

        std::string connection_string = "host=" + host +
                                      " dbname=" + dbname +
                                      " user=" + user +
                                      " password=" + password +
                                      " port=" + std::to_string(sqlport);

        pqxx::connection conn(connection_string);

        if (conn.is_open()) {

            pqxx::work txn(conn);

            pqxx::result result = txn.exec(query);

            txn.commit();
            conn.close();

            return result;
        } else {
            std::cerr << "Failed to open database" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return empty;
    }

    return empty;
}
