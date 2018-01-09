#ifndef PUPPER_LOGIN_FORM
#define PUPPER_LOGIN_FORM

#include <cursesf.h>
#include <cursslk.h>
#include <vector>

class Login : public NCursesForm
{
public:
	Login();
	~Login();
	std::string get_user_name();

private:
	int virtualize(int c);
	std::vector<NCursesFormField*> fields_;
	Soft_Label_Key_Set slks_;
	bool complete_;
};


#if 0
class PupperLoginForm : public NCursesForm
{
private:
  NCursesFormField **F;
  int complete;
  int virtualize(int c);

public:z
  PupperLoginForm();

  int virtualize(int c);
  std::string getUserName(void);
  
};
#endif

    //types: classes, enums, and aliases (using)
    //constructors, assignments, destructor
    //functions
    //data

#endif
