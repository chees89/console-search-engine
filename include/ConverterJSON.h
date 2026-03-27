#pragma once
#include <vector>
#include <string>
#include "nlohmann/json.hpp"

class ConverterJSON {
    public:
    ConverterJSON() = default;
    std::vector<std::string> GetTextDocuments();
    int GetResponseLimit();
    std::vector<std::string> GetRequests();
    void putAnswers(std::vector<std::vector<std::pair<int,float>>>answers);

private:
    nlohmann::json loadConfig();
};
