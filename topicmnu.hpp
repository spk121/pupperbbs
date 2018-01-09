// -*- whitespace-line-column: 132; indent-tabs-mode: t; c-file-style: "stroustrup"; tab-width: 4;  -*-
#ifndef PUPPER_TOPIC_MENU_H
#define PUPPER_TOPIC_MENU_H

#include <cursesm.h>
#include <cursslk.h>
#include <vector>
#include "topic.hpp"

class PupperTopicMenu : public NCursesMenu
{
public:
	PupperTopicMenu(Topics& topics);
	~PupperTopicMenu();
	NCursesMenuItem* operator()();
	bool is_quit();
	int get_topic_id();

private:
	int virtualize(int c);

	Topics& topics_;
	std::vector<NCursesMenuItem*> I;
	Soft_Label_Key_Set slks_;
	bool fn_key_quit_;
	NCursesMenuItem *item_cur_;
};

#endif
