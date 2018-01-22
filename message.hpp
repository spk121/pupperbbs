// -*- whitespace-line-column: 132; indent-tabs-mode: t; c-file-style: "stroustrup"; tab-width: 4;  -*-
#ifndef PUPPER_MESSAGE_HPPP
#define PUPPER_MESSAGE_HPPP

#include <string>
#include <vector>

struct Message {
	Message();
    Message(int& _id,
        std::string& _sender, std::string& _recipient,
        int& _topic_id, std::string& _subject, std::vector<std::string>& _text);

    int id;
    std::string sender;
    std::string recipient;
    int topic_id;
    std::string subject;
    std::vector<std::string> text;
};

#endif
