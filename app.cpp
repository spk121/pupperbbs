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
#include "pfile.hpp"
#include "filemenu.hpp"

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

		else if (mainMenu.is_read() || mainMenu.is_write()) {
			Topics topics = db.get_topics();

			PupperTopicMenu ptm(topics);
			ptm();
			if (ptm.is_quit())
				goto at_main_menu;
			auto msg_headers = db.get_message_headers_list(ptm.get_topic_id());
			if (mainMenu.is_read()) {
				PupperMessageListMenu mlm{msg_headers};
				mlm();
				if (mlm.is_quit())
					goto at_main_menu;
				int msg_id = mlm.get_msg_id();
				Message msg = db.get_message(msg_id);
				PupperMessageViewer pmv{msg};
				pmv.go();
			}
			else {
				PupperMessageEditor pme;
				pme();
				if (pme.is_quit())
					goto at_main_menu;
				std::string sto = pme.getTo();
				std::string ssubj = pme.getSubject();
				std::vector<std::string> vtext;
				pme.getText(vtext);
				db.insertMessage(name, sto, ssubj, ptm.get_topic_id(), vtext);
			}
			goto at_main_menu;
		}
		else if (mainMenu.is_download()) {
			std::vector<Pfile> pfiles = db.get_file_list();
			if (pfiles.size() == 0) {
				Pfile tmp = {0, "xxx", "NO FILES AVAILABLE TO DOWNLOAD"};
				pfiles.push_back(tmp);
			}
			PupperFileMenu pfm(pfiles);
			pfm();
			//if (pfiles.size() == 0)
			//	goto at_main_menu;
			int file_id = pfm.get_file_id();
			Root_Window->addstr(3,3, "Sending file by ZMODEM now");
			refresh();
			usleep(1000000);
			Root_Window->addstr(3,3, "Complete!                 ");
			refresh();
			usleep(1000000);
			Root_Window->addstr(3,3, "                          ");
			refresh();
			goto at_main_menu;
		}
		else if (mainMenu.is_upload()) {
			PupperUploadForm puf;
		at_upload_form:
			puf();
			if (puf.is_quit())
				goto at_main_menu;
			std::string name = puf.get_name();
			std::string desc = puf.get_desc();
			if (!db.is_filename_unused(name)) {
				Root_Window->addstr(3,3, "There is already a file by that name");
				refresh();
				usleep(1000000);
				goto at_upload_form;
			}
			Root_Window->addstr(3,3, "Waiting to receive by ZMODEM now");
			refresh();
			usleep(1000000);
			Root_Window->addstr(3,3, "Complete!                       ");
			refresh();
			usleep(1000000);
			Root_Window->addstr(3,3, "                                ");
			refresh();
			goto at_main_menu;
		}
	}
cleanup:
	usleep(100);
	return 0;
}

static PupperApp *app = new PupperApp();
