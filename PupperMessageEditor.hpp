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
private:
    NCursesFormField **fields_;
    Soft_Label_Key_Set slks_;
    int virtualize(int c);

public:
    PupperMessageEditor();
    void On_Unknown_Command(int c) const;
    int b_submit;
    std::string getTo();
    std::string getFrom() { return std::string(); };
    std::string getSubject();
    void getText(std::vector<std::string>& vtext);
};
#endif