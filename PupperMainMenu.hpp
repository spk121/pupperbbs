#ifndef PUPPER_MAIN_MENU_H
#define PUPPER_MAIN_MENU_H

#include <cursesm.h>

class PupperMainMenu : public NCursesMenu
{
  const static int n_items = 3;

 private:
  // NCursesPanel *P;
  NCursesMenuItem** I;

public:
  PupperMainMenu();
};

#endif
