#include "msglist.hpp"
#include <cstring>

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
	: NCursesMenu (20, 80, 1, 0)
	, msg_hdrs_{msg_hdrs}
	, I(0)
	, topic_id_(0)
{
	string key, val;
	char *ckey, *cval;
    for (auto msg: msg_hdrs) {
		key = to_string(msg.id);
		val = msg.subject;
		ckey = strdup(key.c_str());
		cval = strdup(val.c_str());
		I.emplace_back(new SelectItem(ckey, cval));
	}
	I.emplace_back(new NCursesMenuItem());

	InitMenu(I.data(), TRUE, FALSE);
	frame("Message List");
}  

int PupperMessageListMenu::get_msg_id()
{
	
}
