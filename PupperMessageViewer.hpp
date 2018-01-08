#ifndef PUPPER_MESSAGE_VIEWER_HPP
#define PUPPER_MESSAGE_VIEWER_HPP

#include <string>
#include <cursesw.h>
using namespace std;

class PupperMessageViewer : public NCursesWindow
{
private:
    string* from_;
    string* to_;
    string* topic_;
    string* subject_;
    string* text_;

    void render_text(int ystart, int xstart, string* text);
public:
    PupperMessageViewer(string* from, string* to, string* topic, string* subject, string* text);
    
};

#endif