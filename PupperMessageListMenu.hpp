#ifndef PUPPER_MESSAGE_LIST_MENU_H
#define PUPPER_MESSAGE_LIST_MENU_H

#include <cursesm.h>

class PupperMessageListMenu : public NCursesMenu
{
 private:
  NCursesPanel *P;
  NCursesMenuItem** I;

public:
  PupperMessageListMenu();
};

#endif
