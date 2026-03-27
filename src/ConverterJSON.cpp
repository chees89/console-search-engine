#include "ConverterJSON.h"
#include <stdexcept>
#include <vector>
#include <string>
#include "nlohmann/json.hpp"
#include <fstream>

nlohmann::json ConverterJSON::loadConfig() {
    std::ifstream data("config.json");
    if (!data.is_open())
        throw std::runtime_error("config file is missing.");
    
    nlohmann::json json = nlohmann::json::parse(data);
    if (json["config"].empty())
        throw std::runtime_error("config file is empty");
    
    return json;
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    auto config = loadConfig();

    std::vector<std::string> vec;
        
    for (auto& path : config["files"]) {
        std::ifstream file(path.get<std::string>());

        if (!file.is_open()) {
            std::cerr << "File not found: " << path << std::endl;
            continue;
        }

        std::string content((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
            
        vec.push_back(content);
        }
        
        return vec;
}

int ConverterJSON::GetResponseLimit() {
    auto config = loadConfig();

    int result;
    result = config["config"]["max_responses"].get<int>();

    return result;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::ifstream data("requests.json");

    if(!(data.is_open())) {
        throw std::runtime_error("requests file is missing.");
    }
    
    nlohmann::json json = nlohmann::json::parse(data);

    std::vector<std::string> requests;

    for(auto& request : json["requests"]) {
        requests.push_back(request.get<std::string>());
    }

    return requests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int,float>>>answers) {
    nlohmann::json result;
    std::ofstream file("answers.json");

    for(int i = 0; i < answers.size(); i++) {
        std::string num = std::to_string(i + 1);
        std::string requestID = "request" + std::string(3 - (int)num.size(), '0') + num; 
        
        if(answers[i].empty()) {
            result["answers"][requestID]["result"] = false;
        }

        else {
            result["answers"][requestID]["result"] = true;


            for (auto& pair : answers[i]) {
                result["answers"][requestID]["relevance"].push_back({
                    {"docid", pair.first},
                    {"rank", pair.second}
                });
            }
        }
    }

    file << result.dump(4);
}
