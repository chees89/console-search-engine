#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include <exception>
#include <iostream>

int main() {
  try {
    ConverterJSON converter;

    if (!converter.CheckVersion("1.0")) {
      throw std::runtime_error("config.json has incorrect file version");
    }

    std::cout << "Starting " << converter.GetEngineName() << std::endl;

    InvertedIndex index;
    index.UpdateDocumentBase(converter.GetTextDocuments());

    SearchServer server(index, converter.GetResponseLimit());
    auto results = server.search(converter.GetRequests());

    // Преобразуем RelativeIndex в std::pair<int, float> для putAnswers
    std::vector<std::vector<std::pair<int, float>>> answers;
    for (const auto &req_res : results) {
      std::vector<std::pair<int, float>> converted_req;
      for (const auto &rel_idx : req_res) {
        converted_req.push_back(
            {static_cast<int>(rel_idx.doc_id), rel_idx.rank});
      }
      answers.push_back(converted_req);
    }

    converter.putAnswers(answers);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
