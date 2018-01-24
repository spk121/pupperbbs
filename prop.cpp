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

	open_msg_filename = tree.get("pupper.open-msg-filename", "puppermsg.txt");
	db_password = tree.get("pupper.db-password", "invalid_password");
	nlimit = tree.get("pupper.time-limit", 60);
	klimit = tree.get("pupper.k-limit", 200);
	maxbaud = tree.get("pupper.max-baud", 1200);
	mdmstr = tree.get("pupper.modem-string", "ATE0V0M0X1S0=0");
	messages = tree.get("pupper.message-total", 10);
	msgsize = tree.get("pupper.message-size", 2560);
	callsize = tree.get("pupper.callers", 25);
	filepref = tree.get("pupper.file-prefix", "files/");
}
