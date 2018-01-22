// -*- whitespace-line-column: 132; indent-tabs-mode: t; c-file-style: "stroustrup"; tab-width: 4;  -*-
#ifndef PUPPER_MAIN_MENU_H
#define PUPPER_MAIN_MENU_H

#include <cursesm.h>
#include <cursslk.h>

class PupperMainMenu : public NCursesMenu
{
public:
	PupperMainMenu();
	~PupperMainMenu();
	NCursesMenuItem* operator()(void) override;
	bool is_read();
	bool is_write();
	bool is_upload();
	bool is_download();
	bool is_quit();

private:
	int virtualize(int c) override;
	const static int n_items = 5;

	NCursesMenuItem** I;
	Soft_Label_Key_Set slks_;
	bool quit_;
};

#endif
