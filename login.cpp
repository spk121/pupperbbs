#include <string>
#include <boost/algorithm/string/trim.hpp>
#include <cursesapp.h>
#include <cstring>
#include "login.hpp"
#include "form.hpp"

using namespace std;

Login::Login()
	: NCursesForm{9, 36, 8, 22}
	, fields_{}
	, slks_{Soft_Label_Key_Set::Four_Four}
	, complete_{false}
{
	fields_.emplace_back(new Label{1, 1, "PupperBBS"});
	fields_.emplace_back(new Label{3, 1, "What's your name?"});
	fields_.emplace_back(new NCursesFormField{1, 30, 5, 1});
	fields_.emplace_back(new NCursesFormField{});
	InitForm(fields_.data(), TRUE, FALSE);
	set_current(*fields_[2]);

	char *label4 = strdup("F4 Quit");
	slks_[4] = label4;
	free (label4);
	NCursesApplication::getApplication()->push(slks_);
}

Login::~Login()
{
	try {
		NCursesApplication::getApplication()->pop();
		// delete fields_[0];
		// delete fields_[1];
		// delete fields_[2];
		// delete fields_[3];
		// delete fields_[4];
	}
	catch (const NCursesFormException& e)
	{
		move(1,1);
		addstr(e.message);
		refresh();
	}
	//for (auto* f: fields_)
	//	delete f;
}


static const int CMD_QUIT   = MAX_COMMAND + 1;
static const int CMD_ACTION = MAX_COMMAND + 2;

std::string Login::get_user_name()
{
	string name = (fields_[2])->value();
	boost::algorithm::trim(name);
	return name;	
}

int Login::virtualize(int c)
{
    if (c == KEY_F(4))
        return CMD_QUIT;
	else if (c == KEY_ENTER || c == '\n' || c == '\r') {
		// We do a REQ_LAST_FIELD operation to make sure that the current
		// field has the latest keypresses.
		form_driver(form, REQ_LAST_FIELD);
		string name = get_user_name();
		if (name.size() > 0) {
			complete_ = true;
			return CMD_QUIT;
		}
	}
	
    return NCursesForm::virtualize(c);
}

