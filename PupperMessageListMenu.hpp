#ifndef PUPPER_MESSAGE_LIST_MENU_H
#define PUPPER_MESSAGE_LIST_MENU_H

#include <cursesm.h>
#include <map>
#include <string>

class PupperMessageListMenu : public NCursesMenu
{
 private:
  NCursesPanel *P;
  NCursesMenuItem** I;
  int topic_id_;

public:
  PupperMessageListMenu(std::map<int, std::string>& lst);
};

#endif
