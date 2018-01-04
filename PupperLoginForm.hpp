#ifndef PUPPER_LOGIN_FORM
#define PUPPER_LOGIN_FORM

#include <cursesf.h>
#include <string>

class PupperLoginForm : public NCursesForm
{
private:
  NCursesFormField **F;
  int hhh, www;
  int complete;
public:
  PupperLoginForm();

  int virtualize(int c);
  std::string getUserName(void);
  
};


#endif
