#ifndef PUPPER_FORM_HPP
#define PUPPER_FORM_HPP
#include <cursesf.h>

/// A Lable is a form fields that is uneditable
/// and unselectable.
class Label : public NCursesFormField
{
public:
    Label(int y, int x, std::string title);
    inline void OnError(int err) const;    
};
#endif
