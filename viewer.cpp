#include <cursesw.h>
#include "viewer.hpp"

PupperMessageViewer::PupperMessageViewer(string* from, string* to, string* topic, string* subject, string* text)
	: NCursesWindow(20, 78, 1, 1)
	, from_(from)
	, to_(to)
	, topic_(topic)
	, subject_(subject)
	, text_(text)
{
	addstr(0, 0, "From:");
	addstr(0,10, from->c_str());
	addstr(1, 0, "To:");
	addstr(1,10, to->c_str());
	addstr(2, 0, "Topic:");
	addstr(2, 10, topic->c_str());
	addstr(3, 0, "Subject:");
	addstr(3, 10, subject->c_str());

	render_text(6, 2, text);
	refresh();
}

void PupperMessageViewer::render_text(int y, int x, string* text)
{
	string::size_type pos = 0;
	string::size_type prev = 0;
	while ((pos = text->find("\n", prev)) != string::npos)
	{
		addstr(y, x, text->substr(prev, pos - prev).c_str());
		prev = pos + 1;
		y++;
	}
	addstr(y, x, text->substr(prev).c_str());
}
