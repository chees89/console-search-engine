#include "InvertedIndex.h"
#include <map>
#include <mutex>
#include <sstream>
#include <thread>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
  docs = input_docs;
  std::vector<std::thread> threads;
  std::mutex mtx;

  for (int i = 0; i < docs.size(); i++) {
    threads.push_back(std::thread([this, i, &mtx]() {
      std::istringstream stream(docs[i]);
      std::string word;
      std::map<std::string, int> wordCount;

      while (stream >> word) {
        wordCount[word]++;
      }

      std::lock_guard<std::mutex> lock(mtx);
      for (auto &[w, count] : wordCount) {
        freq_dictionary[w].push_back({(size_t)i, (size_t)count});
      }
    }));
  }

  for (auto &t : threads) {
    t.join();
  }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
  if (freq_dictionary.count(word) > 0) {
    return freq_dictionary[word];
  }
  return {};
}
