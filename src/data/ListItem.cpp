#include "ListItem.h"

ListItem::ListItem() : m_name(), m_is_completed(false) {}

const std::string& ListItem::get_name() const {
  return m_name;
}

void ListItem::set_name(const std::string& name) {
  m_name = name;
}

bool ListItem::is_completed() const {
  return m_is_completed;
}

void ListItem::set_is_completed(bool is_completed) {
  m_is_completed = is_completed;
}
