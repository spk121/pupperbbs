#include "PupperLoginForm.hpp"
#include <string>
#include <boost/algorithm/string/trim.hpp>

class Label : public NCursesFormField
{
public:
  Label(const char* title,
        int row, int col)
    : NCursesFormField(1, static_cast<int>(::strlen(title)), row, col) {
      set_value(title);
      options_off(O_EDIT|O_ACTIVE);
  }
};

PupperLoginForm::PupperLoginForm()
  : NCursesForm(9, 36, (lines()-7)/2, (cols() - 36)/2)
  , F(0)
  , complete(FALSE)
{
  F = new NCursesFormField*[4];
  F[0] = new Label("Pupper BBS", 1, 1);
  F[1] = new Label("What's your name?", 3, 1);
  F[2] = new NCursesFormField(1, 30, 5, 1);
  F[3] = new NCursesFormField();

  InitForm(F, TRUE, FALSE);
  // boldframe();
  set_current(*F[2]);
  // position_cursor();
  hhh = height();
  www = width();
}

static const int CMD_QUIT   = MAX_COMMAND + 1;
static const int CMD_ACTION = MAX_COMMAND + 2;

std::string
PupperLoginForm::getUserName(void)
{
  int drvCmnd;
  int err;
  int c;
  int b_action = FALSE; 
  std::string name;
  
  post();
  show();
  refresh();

  while (!b_action && ((drvCmnd = virtualize((c=getKey()))) != CMD_QUIT)) {
    switch((err=driver(drvCmnd))) {
    case E_REQUEST_DENIED:
      On_Request_Denied(c);
      break;
    case E_INVALID_FIELD:
      On_Invalid_Field(c);
      break;
    case E_UNKNOWN_COMMAND:
      if (drvCmnd == CMD_ACTION) {
	// This ensures sure that typed character are moved into the
	// field storage.
	::form_driver(form, REQ_LAST_FIELD);
	
	name = F[2]->value();
	boost::algorithm::trim(name);
	if (name.empty())
	  ::beep();
	else
	  b_action = TRUE;
      } else
	On_Unknown_Command(c);
      break;
    case E_OK:
      break;
    default:
      OnError(err);
    }
  }

  unpost();
  hide();
  refresh();
  return name;
}


#define CTRL_J 0xA
#define CTRL_M 0xD

int PupperLoginForm::virtualize(int c)
{
  if (c == KEY_ENTER
      || c == CTRL_J
      || c == CTRL_M)
    return CMD_ACTION;
  if (c == KEY_F(7))
    return CMD_QUIT;

  return NCursesForm::virtualize(c);
}

