// -*- whitespace-line-column: 132; indent-tabs-mode: t; c-file-style: "stroustrup"; tab-width: 4;	-*-
#include "message.hpp"

using namespace std;

Message::Message()
	: id{}
	, sender{}
	, recipient{}
	, topic_id{}
	, subject{}
	, text{}
{}

Message::Message(int& _id, string& _sender, string& _recipient,
				 int& _topic_id, string& _subject, vector<string>& _text)
	: id{_id}
	, sender{}
	, recipient{}
	, topic_id{_topic_id}
	, subject{}
	, text{}
{
	sender = _sender;
	recipient = _recipient;
	subject = _subject;
	text = _text;
}
