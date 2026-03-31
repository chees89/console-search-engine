#include "SearchServer.h"
#include <algorithm>
#include <map>
#include <set>
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

    std::set<std::string> uniqueWords(allWords.begin(), allWords.end());
    std::vector<std::string> sortedWords(uniqueWords.begin(),
                                         uniqueWords.end());
    std::sort(sortedWords.begin(), sortedWords.end(),
              [this](const std::string &a, const std::string &b) {
                return _index.GetWordCount(a).size() <
                       _index.GetWordCount(b).size();
              });

    if (sortedWords.empty()) {
      result.push_back({});
      continue;
    }

    std::set<size_t> candidateDocs;
    for (auto &entry : _index.GetWordCount(sortedWords[0])) {
      candidateDocs.insert(entry.doc_id);
    }

    for (int i = 1; i < sortedWords.size(); i++) {
      std::set<size_t> wordDocs;
      for (auto &entry : _index.GetWordCount(sortedWords[i])) {
        wordDocs.insert(entry.doc_id);
      }
      std::set<size_t> intersection;
      for (auto id : candidateDocs) {
        if (wordDocs.count(id))
          intersection.insert(id);
      }
      candidateDocs = intersection;
    }

    std::map<size_t, size_t> docRelevance;
    for (auto &w : sortedWords) {
      for (auto &entry : _index.GetWordCount(w)) {
        if (candidateDocs.count(entry.doc_id)) {
          docRelevance[entry.doc_id] += entry.count;
        }
      }
    }

    size_t maxRelevance = 0;
    for (auto &[id, rel] : docRelevance) {
      if (rel > maxRelevance)
        maxRelevance = rel;
    }

    std::vector<RelativeIndex> queryResult;
    for (auto &[id, rel] : docRelevance) {
      queryResult.push_back({id, (float)rel / maxRelevance});
    }

    std::sort(queryResult.begin(), queryResult.end(),
              [](const RelativeIndex &a, const RelativeIndex &b) {
                if (a.rank == b.rank) {
                  return a.doc_id <
                         b.doc_id; // Если ранги равны, первыми идут меньшие ID
                }

                return a.rank > b.rank; // Иначе по убыванию ранга
              });

    if (queryResult.size() > _maxResponses)
      queryResult.resize(_maxResponses);

    result.push_back(queryResult);
  }
  return result;
}
