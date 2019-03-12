#include "ListSerializer.h"
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace {
template <typename Writer>
void serialize_list_item(Writer& writer, const ListItem& item) {
  writer.String("name");
  writer.String(item.get_name().c_str());

  writer.String("is_completed");
  writer.Bool(item.is_completed());
}
}  // namespace

ListSerializer::ListSerializer() : m_json_document() {}

std::string ListSerializer::serialize(const List& list) {
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  writer.StartObject();
  serialize_list_item(writer, list);
  writer.String("items");
  writer.StartArray();
  for (const auto& list_item : list.get_items()) {
    writer.StartObject();
    serialize_list_item(writer, list_item);
    writer.EndObject();
  }
  writer.EndArray();
  writer.EndObject();
  writer.Flush();

  return std::string(buffer.GetString(), buffer.GetLength());
}
