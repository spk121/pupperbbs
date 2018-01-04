#include "PupperMessageListMenu.hpp"

class SelectItem: public NCursesMenuItem
{
public:
  SelectItem(const char *str)
    : NCursesMenuItem(str)
  {}

  bool action() {
    return TRUE;
  }
};

PupperMessageListMenu::PupperMessageListMenu()
  : NCursesMenu (20, 80, 1, 0)
  , I(0)
{
  //  Fetch all the messages from the database
  I = new NCursesMenuItem*[2];
  I[0] = new NCursesMenuItem("1", "A message");
  I[1] = new NCursesMenuItem(); // Terminator

  InitMenu(I, TRUE, TRUE);
  boldframe("Message List");
  
  //P = new NCursesPanel(1, n_items, lines() - 1, 1);
  //boldframe("Main Menu");
  //P->show();
}
