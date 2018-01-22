// -*- whitespace-line-column: 132; indent-tabs-mode: t; c-file-style: "stroustrup"; tab-width: 4;	-*-
#ifndef PUPPER_DB_HPP
#define PUPPER_DB_HPP

#undef _WIN32
#include <mysql.h>
#include <string>
#undef vector
#include <vector>
#include <map>
#include "topic.hpp"
#include "message.hpp"
#include "pfile.hpp"

using namespace std;

class PupperDB {

public:
	PupperDB();
	~PupperDB();
	void connect(std::string password);
	Topics get_topics();
	std::vector<Message> get_message_headers_list(int topic_id);
	int getAndIncrementCallerCount();
	int insertMessage(std::string& from, std::string& to, std::string& subject,
					  int topic_id, std::vector<std::string> text);
	Message get_message(int msg_id);

	std::vector<Pfile> get_file_list();
	int insert_pfile(std::string& name, std::string& desc);
	
private:
	MYSQL *mysql_;

};
#endif
