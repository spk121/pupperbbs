#ifndef PUPPER_PROPERTIES_H
#define PUPPER_PROPERTIES_H

#include <string> 
#include "topic.hpp"

using namespace std;

struct Pup_static
{
	// Load the INI file.  Return false on failure.
	void load(const std::string &filename);

	string open_msg_filename;
	string db_password;
	int nlimit;			/* max connection duration in minutes */
	int klimit;			/* max connection download in kilobytes */
	unsigned callsize;		/* max number of members stored */
	int messages;			/* max number of messages stored */
	int msgsize;			/* max size of any message */
	Topics topics;
	int maxbaud;			/* Max baud rate, lol. */
	std::string mdmstr;		/* Modem initialization string */
	std::string filepref;		/* File download area path */
	std::string errstr;
};

#endif
