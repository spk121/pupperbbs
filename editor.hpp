// -*- whitespace-line-column: 132; indent-tabs-mode: t; c-file-style: "stroustrup"; tab-width: 4;	-*-
#ifndef PUPPER_MESSAGE_EDITOR_HPP
#define PUPPER_MESSAGE_EDITOR_HPP

#include <cursesf.h>
#include <cursslk.h>
#include <vector>
#include <string>

class PupperMessageEditor : public NCursesForm
{
public:
	static const int EDITOR_LINES = 14;
	static const int EDITOR_COLUMNS = 76;

public:
	PupperMessageEditor();
	bool is_quit();
	std::string getTo();
	std::string getSubject();
	void getText(std::vector<std::string>& vtext);

private:
	int virtualize(int c) override;

private:
	int b_submit;
	NCursesFormField **fields_;
	Soft_Label_Key_Set slks_;
};
#endif
