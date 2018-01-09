// -*- whitespace-line-column: 132; indent-tabs-mode: t; c-file-style: "stroustrup"; tab-width: 4;  -*-
#include <cursesapp.h>
#include <cursesf.h>
#include <cstring>
#include <unistd.h>

#include "login.hpp"
#include "mainmenu.hpp"
#include "msglist.hpp"
#include "editor.hpp"
#include "topicmnu.hpp"
#include "prop.hpp"
#include "db.hpp"
#include "viewer.hpp"
#include "topic.hpp"

class PupperApp : public NCursesApplication
{
public:
	PupperApp()
		: NCursesApplication{TRUE} {}
	int run();

protected:
	int titlesize() const { return 1; }
	void title();
	Soft_Label_Key_Set::Label_Layout useSLKs() const {
		return Soft_Label_Key_Set::Four_Four;
	}
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

int PupperApp::run()
{
	Pup_static _ps;
	PupperDB db;

	try {
		_ps.load("pupper.xml");
		db.connect(_ps.db_password);
	}
	catch (const std::runtime_error &e) {
		Root_Window->addstr(0, 0, e.what());
		refresh();
		usleep(2000000);
		endwin();
		return 1;
	}

	// Login
	std::string name;

at_login:
	{
		Login loginForm;
		loginForm();
		name = loginForm.get_user_name();
	}

	if (name.empty())
		return 0;

	int c = db.getAndIncrementCallerCount();

	// Main Loop
	{
		PupperMainMenu mainMenu;

	at_main_menu:
		mainMenu();

		if(mainMenu.is_quit())
			goto cleanup;

		else if (mainMenu.is_read())
		{
			Topics topics = db.get_topics();

			PupperTopicMenu ptm(topics);
			ptm();
			if (ptm.is_quit())
				goto at_main_menu;
			auto msg_headers = db.get_message_headers_list(ptm.get_topic_id());
			PupperMessageListMenu mlm{msg_headers};
			mlm();
			int msg_id = mlm.get_msg_id();
		}
	}
#if 0



	mlm();
	int msg_id = mlm.get_msg_id();

	Message message = db.get_message(msg_id);

	PupperMessageViewer pmv{message];
	pmv();

	if (pmv.is_quit())
		return 0;
	else if (pmv.is_prev())
		;
	else if (pmv.is_next())
		;
	else if (pmv.is_main_menu())
		;
	else if (pmv.is_back())
		continue;
}

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
				db.insertMessage(name, sto, ssubj, topic, vtext);
			}
		}

PupperTopicMenu topicMenu;
PupperMessageListMenu messageListMenu;
PupperMessageDisplay messageDisplay;
loop:
Action a = mainMenu.getAction();
if (a == Action::Quit)
	return 0;

refresh();

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
}
#endif



cleanup:
usleep(10000000);
return 0;
}

static PupperApp *app = new PupperApp();
