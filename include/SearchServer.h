#pragma once
#include "InvertedIndex.h"
#include <string>
#include <vector>

struct RelativeIndex {
  size_t doc_id;
  float rank;
  bool operator==(const RelativeIndex &other) const {
    return (doc_id == other.doc_id && rank == other.rank);
  }
};
class SearchServer {
public:
  SearchServer(InvertedIndex &idx, int maxResponses = 10)
      : _index(idx), _maxResponses(maxResponses) {};

  std::vector<std::vector<RelativeIndex>>
  search(const std::vector<std::string> &queries_input);

private:
  InvertedIndex &_index;
  int _maxResponses;
};
