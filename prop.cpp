#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>
#include "prop.hpp"
namespace pt = boost::property_tree;

void Pup_static::load(const std::string &filename)
{
	// Create empty property tree object
	pt::ptree tree;

	// Parse the XML into the property tree.
	// This can throw.
	pt::read_xml(filename, tree);

	open_msg_filename = tree.get("puppy.open-msg-filename", "puppermsg.txt");
	db_password = tree.get("puppy.db-password", "invalid_password");
	nlimit = tree.get("puppy.time-limit", 60);
	klimit = tree.get("puppy.k-limit", 200);
	maxbaud = tree.get("puppy.max-baud", 1200);
	mdmstr = tree.get("puppy.modem-string", "ATE0V0M0X1S0=0");
	messages = tree.get("puppy.message-total", 10);
	msgsize = tree.get("puppy.message-size", 2560);
	callsize = tree.get("puppy.callers", 25);
	filepref = tree.get("puppy.file-prefix", "files/");

#if 0
	pt::ptree subtree = tree.get_child("puppy.topics");
	for (auto z = subtree.begin(); z != subtree.end(); ++z)
	{
		pt::ptree leaf = (*z).second;
		topics.push_back(PupperTopic{leaf.get<std::string>("name"),
			leaf.get<std::string>("desc")});
	}
	if (topics.empty())
		topics.push_back(PupperTopic{"generic","everything"});
#endif
}
