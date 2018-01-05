#ifndef PUPPER_TOPIC_MENU_H
#define PUPPER_TOPIC_MENU_H

#include <cursesm.h>
#include <vector>
#include "PupperTopics.hpp"

class PupperTopicMenu : public NCursesMenu
{
  const static int N_ITEMS = 5;
  const static int N_COLS = 60;

 private:
  NCursesPanel *P;
  NCursesMenuItem** I;

public:
  PupperTopicMenu(PupperTopics& topics, int n_items, int cols);
};

#endif
