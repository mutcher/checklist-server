#include "DataManager.h"
#include <cassert>

const DataManager::IdType DataManager::INVALID_ID =
    static_cast<DataManager::IdType>(-1);

DataManager::DataManager() : m_data_model() {
  m_data_model.reserve(10);
}

ErrorCode DataManager::get_list(IdType id, List& list) {
  if (id == INVALID_ID) {
    list.set_name("");
    list.set_is_completed(false);
    for (const auto& general_list : m_data_model) {
      list.get_items().push_back(general_list);
    }
    return ErrorCode::ERROR_OK;
  } else if (id < m_data_model.size()) {
    list = m_data_model[id];
    return ErrorCode::ERROR_OK;
  }
  return ErrorCode::ERROR_NOT_FOUND;
}

ErrorCode DataManager::get_list_serialized(IdType id,
                                           std::string& serialized_list) {
  List list;
  auto error_code = get_list(id, list);
  if (error_code == ErrorCode::ERROR_OK) {
    serialized_list = m_serializer.serialize(list);
  }
  return error_code;
}
