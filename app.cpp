#include <cursesapp.h>
#include <cursesf.h>
#include <cstring>
#include <unistd.h>

#include "PupperLoginForm.hpp"
#include "PupperMainMenu.hpp"
#include "PupperMessageListMenu.hpp"
#include "PupperMessageEditor.hpp"
#include "PupperTopicMenu.hpp"
#include "properties.hpp"
#include "PupperDB.hpp"

class PupperApp : public NCursesApplication
{
private:
	pup_static _ps;

protected:
	int titlesize() const { return 1; }
	void title();
	Soft_Label_Key_Set::Label_Layout useSLKs() const {
		return Soft_Label_Key_Set::Four_Four;
	}
	void init_labels(Soft_Label_Key_Set& S) const;

public:
	PupperApp() : NCursesApplication(TRUE) {
		_ps.load("pupper.xml");
	}

	int run();
};

PupperApp *getApplication()
{
	return static_cast<PupperApp *>(NCursesApplication::getApplication());
}

void PupperApp::title()
{
	const char * const titleText = "Pupper BBS";
	const int len = ::strlen(titleText);

	titleWindow->bkgd(screen_titles());
	titleWindow->addstr(0, (titleWindow->cols() - len)/2, titleText);
	titleWindow->noutrefresh();
}

void PupperApp::init_labels(Soft_Label_Key_Set& S) const
{
	// Weirdly, if you set a SLK equal to a string, that sets the label,
	// but if you set it equal to a Justification, it sets the
	// justification.

	// When you set SLK equal to a string, it copies the string and
	// manages its lifetime.
	S[7] = "F7 Quit";
}

enum class Action {
	Quit
};

int PupperApp::run()
{
	// Database
	PupperDB db(_ps.db_password);


	if (!db.ready) {
		move(1,1);
		addstr(db.errstr.c_str());
		refresh();
		usleep(1000000);
	}

	int c = db.getAndIncrementCallerCount();
	int c2 = db.getAndIncrementCallerCount();

	// Login
	std::string name;
	{
		PupperLoginForm loginForm;
		name = loginForm.getUserName();
	}


	if (name.empty())
		return 0;

	// Main Loop
	{
		PupperMainMenu mainMenu;
		NCursesMenuItem *x = mainMenu();
		
		if(x->index() == 4)
			goto cleanup;
		else if (x->index() == 0)
		{
			#if 1
			PupperTopicMenu ptm(_ps.topics, lines(_ps.topics), cols(_ps.topics));
			int topic = ptm()->index();			
			PupperMessageListMenu messageListMenu(topic);
			NCursesMenuItem *msgItem = messageListMenu();
			int msg_id = std::stoi(msgItem->name());
			#endif
		}
		else if (x->index() == 1)
		{
			PupperTopicMenu ptm(_ps.topics, lines(_ps.topics), cols(_ps.topics));
			int topic = ptm()->index();
			PupperMessageEditor pme;
			pme();
			if (pme.b_submit)
			{
				std::string sto = pme.getTo();
				std::string ssubj = pme.getSubject();
				std::vector<std::string> vtext;
				pme.getText(vtext);
			}
		}
#if 0    
		PupperTopicMenu topicMenu;
		PupperMessageListMenu messageListMenu;
		PupperMessageDisplay messageDisplay;
	loop:
		Action a = mainMenu.getAction();
		if (a == Action::Quit)
			return 0;
#endif
		refresh();
#if 0    
		else if (action == ACTION_READ_MESSAGES)
			{
	int topic = topicMenu.getTopic();
	if (topic < 0)
		goto loop;
			loop2:
	int msg_id = messageListmenu(topic);
	if (msg_id < 0)
		goto loop;
	messageDisplay.show(msg_id);
	goto loop2;
			}
		else if (action == ACTION_WRITE_MESSAGE)
			{
	messageEditor.go();
	goto loop;
			}
		else if (action == ACTION_DOWNLOAD)
			{
			loop3:
	std::string fname = fileDownloadList.go();
	if (fname.empty())
		goto loop;
	fileDownload.go(fname);
	goto loop3;
			}
		else if (action == ACTION_UPLOAD)
			{
	fileUploader.go();
	goto loop;
			}
#endif
	}

cleanup:
	usleep(10000000);
	return 0;
}

static PupperApp *app = new PupperApp();
