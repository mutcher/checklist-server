#pragma once
#include "List.h"
#include "ListSerializer.h"

enum class ErrorCode {
  ERROR_OK,
  ERROR_NOT_FOUND,
};

class DataManager {
 public:
  DataManager();
  typedef size_t IdType;
  static const IdType INVALID_ID;

  // if id equals to INVALID_ID, list of lists will be returned
  ErrorCode get_list(IdType id, List& list);
  // if id equals to INVALID_ID, list of lists will be returned
  ErrorCode get_list_serialized(IdType id, std::string& serialized_list);

 private:
  std::vector<List> m_data_model;
  ListSerializer m_serializer;
};
