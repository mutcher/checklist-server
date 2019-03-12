#include "List.h"

List::List() : ListItem(), m_items() {}

std::vector<ListItem>& List::get_items() {
  return m_items;
}

const std::vector<ListItem>& List::get_items() const {
  return m_items;
}
