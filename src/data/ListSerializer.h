#pragma once
#include <rapidjson/document.h>
#include <string>

#include "List.h"

class ListSerializer {
 public:
  ListSerializer();

  std::string serialize(const List& list);

 private:
  rapidjson::Document m_json_document;
};
