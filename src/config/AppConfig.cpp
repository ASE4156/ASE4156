#include "AppConfig.h"
#include <fstream>

std::string get_openai_api_key() {
    std::string key;
    std::ifstream env_file(".env");
    if (env_file) {
        std::string line;
        while (std::getline(env_file, line)) {
            if (line.find("OPENAI_API_KEY=") == 0) {
                key = line.substr(15);  // 14 is the length of "OPENAI_API_KEY="
            }
        }
    }
    env_file.close();
    return key;
}