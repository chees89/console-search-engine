#include "SearchServer.h"
#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::vector<RelativeIndex>>
SearchServer::search(const std::vector<std::string> &queries_input) {
  std::vector<std::vector<RelativeIndex>> result;

  for (auto &query : queries_input) {
    std::istringstream stream(query);
    std::string word;
    std::vector<std::string> allWords;

    while (stream >> word) {
      allWords.push_back(word);
    }

    std::map<size_t, size_t> docRelevance;

    for (auto &w : allWords) {
      auto entries = _index.GetWordCount(w);
      for (auto &entry : entries) {
        docRelevance[entry.doc_id] += entry.count;
      }
    }

    size_t maxRelevance = 0;
    for (auto &[doc_id, rel] : docRelevance) {
      if (rel > maxRelevance)
        maxRelevance = rel;
    }

    std::vector<RelativeIndex> queryResult;
    for (auto &[doc_id, rel] : docRelevance) {
      queryResult.push_back({doc_id, (float)rel / maxRelevance});
    }

    std::sort(queryResult.begin(), queryResult.end(),
              [](const RelativeIndex &a, const RelativeIndex &b) {
                return a.rank > b.rank;
              });

    result.push_back(queryResult);
  }
  return result;
}
