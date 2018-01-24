// -*- whitespace-line-column: 132; indent-tabs-mode: t; c-file-style: "stroustrup"; tab-width: 4;  -*-
#include "topicmnu.hpp"
#include <cursesm.h>
#include <cursesapp.h>
#include <cstring>

class TopicItem: public NCursesMenuItem
{
public:
	TopicItem(Topic& topic)
		: NCursesMenuItem(topic.name.c_str(), topic.description.c_str())
		{}

	bool action() {
		return TRUE;
	}
};

PupperTopicMenu::PupperTopicMenu(Topics& topics)
	: NCursesMenu{topics_line_count(topics) + 2, topics_col_count(topics) + 3,
		10 - topics_line_count(topics)/2, 40 - topics_col_count(topics)/2}
	, topics_{topics}
	, I{}
	, slks_{Soft_Label_Key_Set::Four_Four}
	, fn_key_quit_{false}
{
	for (auto& topic: topics)
		I.emplace_back(new TopicItem(topic));
	I.emplace_back(new NCursesMenuItem());

	InitMenu(I.data(), TRUE, FALSE);
	frame("TOPIC MENU");

	char *label4 = strdup("F4 Quit");
	slks_[4] = label4;
	free (label4);
	NCursesApplication::getApplication()->push(slks_);
}

PupperTopicMenu::~PupperTopicMenu()
{
	NCursesApplication::getApplication()->pop();
}

NCursesMenuItem* PupperTopicMenu::operator()()
{
	item_cur_ = NCursesMenu::operator()();
	return item_cur_;
}

bool PupperTopicMenu::is_quit()
{
	if (fn_key_quit_)
		return true;
	return item_cur_ == nullptr;
}

int PupperTopicMenu::get_topic_id()
{
	int pos = item_cur_->index();
	return topics_.at(pos).id;
}

static int CMD_QUIT = MAX_COMMAND + 1;

int PupperTopicMenu::virtualize(int c)
{
	if (c == KEY_F(4)) {
		fn_key_quit_ = true;
		return CMD_QUIT;
	}

	return NCursesMenu::virtualize(c);
}
