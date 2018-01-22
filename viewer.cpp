// -*- whitespace-line-column: 132; indent-tabs-mode: t; c-file-style: "stroustrup"; tab-width: 4;  -*-
#include <cursesw.h>
#include "viewer.hpp"

PupperMessageViewer::PupperMessageViewer(Message &msg)
	: NCursesPanel(20, 78, 1, 1)
{
	addstr(0, 0, "From:");
	addstr(0,10, msg.sender.c_str());
	addstr(1, 0, "To:");
	addstr(1,10, msg.recipient.c_str());
	addstr(3, 0, "Subject:");
	addstr(3, 10, msg.subject.c_str());

	render_text(6, 2, msg.text);
	refresh();
}

void PupperMessageViewer::go()
{
	getch();
	hide();
}

void PupperMessageViewer::render_text(int y, int x, std::vector<std::string>& text)
{
	for(auto line: text)
		addstr(y++, x, line.c_str());
}
