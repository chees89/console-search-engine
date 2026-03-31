#pragma once
#include "nlohmann/json.hpp"
#include <string>
#include <vector>

class ConverterJSON {
public:
  std::string GetEngineName();
  bool CheckVersion(const std::string &app_version);

  ConverterJSON() = default;
  std::vector<std::string> GetTextDocuments();
  int GetResponseLimit();
  std::vector<std::string> GetRequests();
  void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);

private:
  nlohmann::json loadConfig();
};
