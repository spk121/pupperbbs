#include <string>
#include "PupperApp.hpp"

using namespace std;

void PupperApp::title()
{
  string titleText = "Pupper BBS";

    titleWindow->bkgd(screen_titles());
    titleWindow->addstr(0, (titleWindow->cols() - titleText.size())/2, titleText.c_str());
    titleWindow->noutrefresh();
}


int PupperApp::run()
{
    refresh();
    return 0;
}