#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {
  ConverterJSON converter;

  InvertedIndex index;
  index.UpdateDocumentBase(converter.GetTextDocuments());

  SearchServer server(index);
  auto results = server.search(converter.GetRequests());

  converter.putAnswers(results);

  return 0;
}
