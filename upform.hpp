// -*- whitespace-line-column: 132; indent-tabs-mode: t; c-file-style: "stroustrup"; tab-width: 4;	-*-
#ifndef PUPPER_UPLOAD_FORM_H
#define PUPPER_UPLOAD_FORM_H

#include <cursesf.h>
#include <cursslk.h>
#include <vector>
#include <string>

class PupperUploadForm : public NCursesForm
{
public:
	static const int EDITOR_LINES = 14;
	static const int EDITOR_COLUMNS = 76;

public:
	PupperUploadForm();
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

