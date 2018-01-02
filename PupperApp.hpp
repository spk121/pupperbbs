#ifndef PUPPER_APP
#define PUPPER_APP
#include <cursesapp.h>

class PupperApp : public NCursesApplication
{
protected:
    int titlesize() const { return 1; }
    void title();

public:
    PupperApp()
      : NCursesApplication(TRUE)
      {}

    int run();
};

#endif