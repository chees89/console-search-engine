#include "ConverterJSON.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

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

  for (auto &path : config["files"]) {
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

std::string ConverterJSON::GetEngineName() {
  auto config = loadConfig();
  if (config["config"].contains("name")) {
    return config["config"]["name"].get<std::string>();
  }
  return "Unknown Engine";
}

bool ConverterJSON::CheckVersion(const std::string &app_version) {
  auto config = loadConfig();
  if (config["config"].contains("version") &&
      config["config"]["version"].get<std::string>() == app_version) {
    return true;
  }
  return false;
}

int ConverterJSON::GetResponseLimit() {
  auto config = loadConfig();

  if (config["config"].contains("max_responses")) {
    if (config["config"]["max_responses"].is_number()) {
      return config["config"]["max_responses"].get<int>();
    } else if (config["config"]["max_responses"].is_string()) {
      return std::stoi(config["config"]["max_responses"].get<std::string>());
    }
  }

  return 10;
}

std::vector<std::string> ConverterJSON::GetRequests() {
  std::ifstream data("requests.json");

  if (!(data.is_open())) {
    throw std::runtime_error("requests file is missing.");
  }

  nlohmann::json json = nlohmann::json::parse(data);

  std::vector<std::string> requests;

  for (auto &request : json["requests"]) {
    requests.push_back(request.get<std::string>());
  }

  return requests;
}

void ConverterJSON::putAnswers(
    std::vector<std::vector<std::pair<int, float>>> answers) {
  nlohmann::json result;
  std::ofstream file("answers.json");

  for (int i = 0; i < answers.size(); i++) {
    std::string num = std::to_string(i + 1);
    std::string requestID =
        "request" + std::string(3 - (int)num.size(), '0') + num;

    if (answers[i].empty()) {
      result["answers"][requestID]["result"] = false;
    } else {
      result["answers"][requestID]["result"] = true;

      if (answers[i].size() == 1) {
        // Если результат один — создаем объект
        result["answers"][requestID]["relevance"] = {
            {"docid", answers[i][0].first}, {"rank", answers[i][0].second}};
      } else {
        // Если результатов несколько — создаем массив
        for (auto &pair : answers[i]) {
          result["answers"][requestID]["relevance"].push_back(
              {{"docid", pair.first}, {"rank", pair.second}});
        }
      }
    }
  }
  file << result.dump(4);
}
