#pragma once
#include <vector>

#include "ListItem.h"

class List : public ListItem {
 public:
  List();
  std::vector<ListItem>& get_items();
  const std::vector<ListItem>& get_items() const;

 private:
  std::vector<ListItem> m_items;
};