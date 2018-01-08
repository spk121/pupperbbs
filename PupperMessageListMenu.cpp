#include "PupperMessageListMenu.hpp"
#include <cstring>

class SelectItem: public NCursesMenuItem
{
public:
  SelectItem(const char *key, const char *val)
    : NCursesMenuItem(key, val)
  {}

  bool action() {
    return TRUE;
  }
};

PupperMessageListMenu::PupperMessageListMenu(std::map<int, std::string>& lst)
  : NCursesMenu (20, 80, 1, 0)
  , I(0)
  , topic_id_(0)
{

  I = new NCursesMenuItem*[lst.size() + 1];
  int i = 0;
  for(auto x: lst)
  {
    std::string key = std::to_string(x.first);
    I[i++] = new SelectItem(strdup(key.c_str()), strdup(x.second.c_str()));
  }
  I[i++] = new NCursesMenuItem(); // Terminator

  InitMenu(I, TRUE, TRUE);
  boldframe("Message List");
  
  //P = new NCursesPanel(1, n_items, lines() - 1, 1);
  //boldframe("Main Menu");
  //P->show();
}
