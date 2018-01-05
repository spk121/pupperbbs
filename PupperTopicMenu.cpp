#include "PupperTopicMenu.hpp"
#include <cursesm.h>



class TopicItem: public NCursesMenuItem
{
public:
  TopicItem(PupperTopic& topic)
    : NCursesMenuItem(topic.name.c_str(), topic.description.c_str())
  {}

  bool action() {
    return TRUE;
  }
};

PupperTopicMenu::PupperTopicMenu(PupperTopics& topics, int n_items, int cols)
    : NCursesMenu(n_items + 2, cols + 3, 10 - n_items/2, 40 - cols/2)
    , I(0)
{
    I = new NCursesMenuItem*[n_items + 1];

    int i = 0;
    for (auto& topic: topics) {
        I[i] = new TopicItem(topic);
        i++;
        if (i > n_items)
            break;
    }
    I[i] = new NCursesMenuItem();

    InitMenu(I, TRUE, TRUE);
    boldframe("TOPIC MENU");  
}