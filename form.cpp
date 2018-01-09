#include "form.hpp"

Label::Label(int y, int x, std::string title)
    : NCursesFormField{1, title.size(), y, x}
{
    set_value(title.c_str());
    options_off(O_EDIT | O_ACTIVE);
}


inline void Label::OnError(int err) const
{
    if (err!=E_OK)
       throw new NCursesFormException (err);
}