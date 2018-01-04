#include "PupperMainMenu.hpp"

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

PupperMainMenu::PupperMainMenu()
  : NCursesMenu (n_items + 2, 30, (lines()-10)/2, (cols()-30)/2)
  , I(0)
{
  I = new NCursesMenuItem*[1 + n_items];
  I[0] = new SelectItem("Read messages");
  I[1] = new SelectItem("Write a message");
  I[2] = new SelectItem("Download files");
  I[3] = new SelectItem("Upload a file");
  I[4] = new SelectItem("Quit");
  I[5] = new NCursesMenuItem(); // Terminator

  InitMenu(I, TRUE, TRUE);
  boldframe("MAIN MENU");
  
  //P = new NCursesPanel(1, n_items, lines() - 1, 1);
  //boldframe("Main Menu");
  //P->show();
}
