// -*- whitespace-line-column: 132; indent-tabs-mode: t; c-file-style: "stroustrup"; tab-width: 4;  -*-
#include "mainmenu.hpp"
#include <cstring>
#include <cursesapp.h>

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
	: NCursesMenu (n_items + 2, 30, 11 - (n_items+2)/2, 40 - (32/2))
	, I(0)
	, slks_{Soft_Label_Key_Set::Four_Four}
	, quit_{false}
{
	I = new NCursesMenuItem*[1 + n_items];
	I[0] = new SelectItem("Read messages");
	I[1] = new SelectItem("Write a message");
	I[2] = new SelectItem("Download files");
	I[3] = new SelectItem("Upload a file");
	I[4] = new SelectItem("Quit");
	I[5] = new NCursesMenuItem(); // Terminator

	InitMenu(I, TRUE, TRUE);

}

PupperMainMenu::~PupperMainMenu()
{
	NCursesApplication::getApplication()->pop();

}

NCursesMenuItem* PupperMainMenu::operator()(void)
{
	frame("MAIN MENU");
	char *label4 = strdup("F4 Quit");
	slks_[4] = label4;
	free (label4);
	NCursesApplication::getApplication()->push(slks_);

	return NCursesMenu::operator()();
}

static const int CMD_QUIT   = MAX_COMMAND + 1;

int PupperMainMenu::virtualize(int c)
{
	if (c == KEY_F(4)) {
		quit_ = true;
		return CMD_QUIT;
	}
	return NCursesMenu::virtualize(c);
}

bool PupperMainMenu::is_read()
{
	return !quit_ && current_item()->index() == 0;
}

bool PupperMainMenu::is_write()
{
	return !quit_ && current_item()->index() == 1;
}

bool PupperMainMenu::is_upload()
{
	return !quit_ && current_item()->index() == 3;
}

bool PupperMainMenu::is_download()
{
	return !quit_ && current_item()->index() == 2;
}

bool PupperMainMenu::is_quit()
{
	return quit_ || (current_item()->index() == 4);
}
