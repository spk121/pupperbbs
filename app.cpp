// -*- whitespace-line-column: 132; indent-tabs-mode: t; c-file-style: "stroustrup"; tab-width: 4;  -*-
#include <cursesapp.h>
#include <cursesf.h>
#include <cstring>
#include <unistd.h>
#include <map>
#include <string>
extern "C" {
#include <zmodem.h>
};

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

std::map<std::string, int> download;
#define DL_DOWNLOAD_COMPLETE 0
#define DL_FILENAME_TOO_LONG 1
#define DL_FILENAME_INVALID 2
#define DL_FILE_SIZE_TOO_LARGE 3
#define DL_FILE_SIZE_TOO_SMALL 4
#define DL_FILE_EXISTS 5
#define DL_TIME_TOO_FAR_IN_FUTURE 6
#define DL_TIME_TOO_FAR_IN_PAST 7
#define DL_DOWNLOAD_INCOMPLETE 8

PupperDB *pdb;


static bool
approver_cb(const char *filename, size_t size, time_t date)
{
	// Empty filenames
	if (filename == NULL || strlen(filename) == 0)
		return false;

	// Duplicate filenames
	if (download.find(filename) != download.end())
		return false;

	// Invalid filenames
	if (strlen(filename) > 13) {
		download.emplace(filename, DL_FILENAME_TOO_LONG);
		return false;
	}

	size_t spnsz
		= strspn(filename,
				 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwzyz0123456789!#$%&'()-@^_`{}~.");

	if (spnsz < strlen(filename) || filename[0] == '.') {
		download.emplace(filename, DL_FILENAME_INVALID);
		return false;
	}

	int dotcount = 0;
	for (size_t i = 0; i < strlen(filename); i ++) {
		if (filename[i] == '.')
			dotcount ++;
	}
	if (dotcount > 1) {
		download.emplace(filename, DL_FILENAME_INVALID);
		return false;
	}

	if (size > 0xFFFF) {
		download.emplace(filename, DL_FILE_SIZE_TOO_LARGE);
		return false;
	}

	if (size < 0xF) {
		download.emplace(filename, DL_FILE_SIZE_TOO_SMALL);
		return false;
	}

	std::string fnamestr = filename;
	if (pdb->pfile_exists(fnamestr)) {
		download.emplace(filename, DL_FILE_EXISTS);
		return false;
	}
	time_t tm = time(NULL);
	if (tm != (time_t)(-1) && date > tm + 24*60*60) {
		download.emplace(filename, DL_TIME_TOO_FAR_IN_FUTURE);
		return false;
	}

	if (date < 2) {
		download.emplace(filename, DL_TIME_TOO_FAR_IN_PAST);
		return false;
	}

	// No more than 14 files at a time
	if (download.size() > 14)
		return false;

	download.emplace(filename, DL_DOWNLOAD_INCOMPLETE);
	return true;
}

static
bool tick_cb(const char *fname, long bytes_sent, long bytes_total, long last_bps, int min_left, int sec_left)
{
	if (min_left > 10)
		return false;
	return true;
  return true;
}

static void
complete_cb(const char *filename, int result, size_t size, time_t date)
{
  if (result == RZSZ_NO_ERROR)
	  download[filename] = DL_DOWNLOAD_COMPLETE;
}


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
	pdb = &db;

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
			// Here we kick off a zmodem sender process for the given
			// file.

			// There can't be anything written to stdout because it would
			// interfere with the zmodem process.

			// Display a message when complete.
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
#if 0
			PupperUploadForm ppuf;
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
#endif
			Root_Window->addstr(3,3, "Waiting to receive by ZMODEM now");
			refresh();
			sleep(1);
			size_t bytes = zmodem_receive("files", /* use files directory */
										  approver_cb,
										  tick_cb,
										  complete_cb,
										  0,
										  RZSZ_FLAGS_NONE);

			// Here we kick of a zmodem receiver.  Figure it is a two-column
			// table.

			// There can't be anything written to stdout because it would
			// interfere with the zmodem process.

			// There are only maybe 16 rows max, so after 16 files, uploads are
			// rejected.

			// After the uploads are finished, there the two-column table will
			// have filenames and reasons for rejection for rejected files.
			// Descriptions are "greyed out".
			// For accepted files, there second column can be entered
			// like a form for each successfully uploaded file.

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
