// -*- whitespace-line-column: 132; indent-tabs-mode: t; c-file-style: "stroustrup"; tab-width: 4;  -*-
#include "upform.hpp"

#include <cursesapp.h>
#include <boost/algorithm/string/trim.hpp>
#include <vector>
#include <string>

/// A Lable is a form fields that is uneditable
/// and unselectable.
class LabelFormField : public NCursesFormField
{
public:

  LabelFormField(const char* title,
        int row, int col)
    : NCursesFormField(1, static_cast<int>(::strlen(title)), row, col) {
      set_value(title);
      options_off(O_EDIT|O_ACTIVE);
  }
};


PupperUploadForm::PupperUploadForm()
	: NCursesForm{EDITOR_LINES + 6, EDITOR_COLUMNS + 2, 1, 1}
	, fields_(nullptr)
	, slks_(Soft_Label_Key_Set::Four_Four)
	, b_submit(FALSE)
{
    fields_ = new NCursesFormField*[8];

    // addressee entry 
    fields_[0] = new LabelFormField("To:", 0, 0);
    fields_[1] = new NCursesFormField(1, 20, 0, 10);

    // subject entry
    fields_[2] = new LabelFormField("Subject:", 2, 0);
    fields_[3] = new NCursesFormField(1, 36, 2, 10);

    // Some text
    fields_[4] = new LabelFormField("Press <f4> to quit", 0, 55);
    fields_[5] = new LabelFormField("Press <f8> to save", 1, 55);

    // multiline text entry
    fields_[6] = new NCursesFormField(EDITOR_LINES, EDITOR_COLUMNS, 4, 0);
    // fields_[4]->options_off(O_STATIC);
    // fields_[4]->set_maximum_growth(16);
    //fields_[6]->set_background(A_NORMAL);

    fields_[7] = new NCursesFormField();

    InitForm(fields_, TRUE, TRUE);
    fields_[6]->set_pad_character(' ');

    // Set up some new edit soft keys
    slks_[1] = "";
    slks_[4] = "F4 Quit";
    slks_[8] = "F8 Save";
    NCursesApplication::getApplication()->push(slks_);

}

static const int CMD_QUIT = MAX_COMMAND + 1;
static const int CMD_ACTION = MAX_COMMAND + 2;

int PupperUploadForm::virtualize(int c)
{
    if (c == KEY_F(4))
    {
        b_submit = FALSE;
        return CMD_QUIT;
    }
    else if (c == KEY_F(8))
    {
        b_submit = TRUE;
        // The following command ensures that all cached keypresses
        // end up in the field storage.
        ::form_driver(form, REQ_LAST_FIELD);
        return CMD_QUIT;
    }
    return NCursesForm::virtualize(c);
}

bool PupperUploadForm::is_quit()
{
	return !b_submit;
}
std::string PupperUploadForm::getTo()
{
    NCursesFormField* field = fields_[1];
    std::string s(field->value());
    boost::algorithm::trim(s);
    return s;
}

std::string PupperUploadForm::getSubject()
{
    NCursesFormField* field = fields_[3];
    std::string s(field->value());
    boost::algorithm::trim(s);
    return s;
}

void PupperUploadForm::getText(std::vector<std::string>& vtext)
{
    vtext.clear();

    NCursesFormField* field = fields_[6];
    std::string s(field->value());
    for (int i = 0; i < EDITOR_LINES; i ++)
    {
        std::string line = s.substr(i * EDITOR_COLUMNS, EDITOR_COLUMNS);
        vtext.emplace_back(boost::algorithm::trim_right_copy(line));
        //vtext.emplace_back(line);
    }
    int i = EDITOR_LINES;
    while (vtext[i-1].empty() && (i - 1 > 0))
        i--;
    vtext.resize(i);
    vtext.shrink_to_fit();
}
