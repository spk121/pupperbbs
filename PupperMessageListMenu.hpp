#ifndef PUPPER_MESSAGE_LIST_MENU_H
#define PUPPER_MESSAGE_LIST_MENU_H

#include <cursesm.h>

class PupperMessageListMenu : public NCursesMenu
{
 private:
  NCursesPanel *P;
  NCursesMenuItem** I;
  int topic_id_;

public:
  PupperMessageListMenu(int topic_id);
};

#endif
