#pragma once
#include <string>

class ListItem {
 public:
  ListItem();
  const std::string& get_name() const;
  void set_name(const std::string& name);

  bool is_completed() const;
  void set_is_completed(bool is_completed);

 private:
  std::string m_name;
  bool m_is_completed;
};