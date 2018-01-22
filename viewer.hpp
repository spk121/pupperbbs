// -*- whitespace-line-column: 132; indent-tabs-mode: t; c-file-style: "stroustrup"; tab-width: 4;  -*-
#ifndef PUPPER_MESSAGE_VIEWER_HPP
#define PUPPER_MESSAGE_VIEWER_HPP

#include <string>
#include <vector>
#include <cursesp.h>
#include "message.hpp"

class PupperMessageViewer : public NCursesPanel
{
public:
	PupperMessageViewer(Message &msg);
	void go();
private:	
    void render_text(int ystart, int xstart, std::vector<std::string>& text);
};

#endif
