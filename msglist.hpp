// -*- whitespace-line-column: 132; indent-tabs-mode: t; c-file-style: "stroustrup"; tab-width: 4;  -*-
#ifndef PUPPER_MESSAGE_LIST_MENU_H
#define PUPPER_MESSAGE_LIST_MENU_H

#include <cursesm.h>
#include <vector>
#include "message.hpp"

class PupperMessageListMenu : public NCursesMenu
{
public:
	PupperMessageListMenu(std::vector<Message>& msg_hdrs);
	int get_msg_id();
	bool is_quit();
	int virtualize(int c) override;
	
private:
	std::vector<Message>& msg_hdrs_;
	std::vector<NCursesMenuItem*> I;
	int topic_id_;
	bool quit_;
};

#endif
