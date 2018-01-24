// -*- whitespace-line-column: 132; indent-tabs-mode: t; c-file-style: "stroustrup"; tab-width: 4;  -*-
#include "msglist.hpp"
#include <cstring>
#include <sstream>
#include <iomanip>

using namespace std;

class SelectItem: public NCursesMenuItem
{
public:
	SelectItem(const char *key, const char *val)
		: NCursesMenuItem(key, val)
		{}

	bool action() {
		return TRUE;
	}
};

PupperMessageListMenu::PupperMessageListMenu(std::vector<Message>& msg_hdrs)
	: NCursesMenu{20, 80, 1, 0}
	, msg_hdrs_{msg_hdrs}
	, I(0)
	, topic_id_{0}
	, quit_{false}
{
	string key, val;
	char *ckey, *cval;
	std::stringstream sstr;
    for (auto msg: msg_hdrs) {
		key = to_string(msg.id);
		sstr.str("");
		// sstr << msg.sender;
		sstr << std::right << std::setw(12) << msg.sender;
		sstr << ' ' << std::right << std::setw(12) << msg.recipient;
		sstr << "  " << std::left << std::setw(36) << msg.subject;
		ckey = strdup(key.c_str());
		cval = strdup(sstr.str().c_str());
		I.emplace_back(new SelectItem(ckey, cval));
	}
	I.emplace_back(new NCursesMenuItem());

	InitMenu(I.data(), TRUE, FALSE);
	frame("Message List");
}

bool PupperMessageListMenu::is_quit()
{
	return quit_;
}

static const int CMD_QUIT   = MAX_COMMAND + 1;

int PupperMessageListMenu::virtualize(int c)
{
	if (c == KEY_F(4)) {
		quit_ = true;
		return CMD_QUIT;
	}
	return NCursesMenu::virtualize(c);
}

int PupperMessageListMenu::get_msg_id()
{
	if (quit_)
		return -1;
	return msg_hdrs_[current_item()->index()].id;
	
}
