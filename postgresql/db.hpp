#pragma once

#include <iostream>
#include <pqxx/pqxx>


std::string sql_return(const std::string& query);
pqxx::result sql_return_result(const std::string& query);