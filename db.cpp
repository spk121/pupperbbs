#include "db.hpp"
#include <sstream>
#include <boost/format.hpp>
#include <map>

std::map<int, std::string> messageList;


PupperDB::PupperDB()
    : mysql_{nullptr}
{
    mysql_ = mysql_init(NULL);
    if (mysql_ == NULL)
        throw new std::runtime_error("out of memory");
}

PupperDB::~PupperDB()
{
    mysql_close(mysql_);
}

void PupperDB::connect(std::string password)
{
    MYSQL *ret = mysql_real_connect(mysql_, NULL, "pupper", password.c_str(), "pupper", 0, NULL, 0);
    if (ret == NULL)
        throw new std::runtime_error(mysql_error(mysql_));
}

Topics PupperDB::get_topics()
{
    int ret = mysql_query(mysql_, "SELECT id, name, description FROM topic;");
    if (ret != 0)
        throw new std::runtime_error(mysql_error(mysql_));

     // The query worked, so let's make a message list
    MYSQL_RES* result = mysql_store_result(mysql_);
    if (result == nullptr) {
        if (mysql_errno(mysql_) != 0)
            throw new std::runtime_error(mysql_error(mysql_));
        else
            return Topics();
    }

    if (mysql_num_fields(result) != 3) {
        mysql_free_result(result);
        throw new std::runtime_error("the topics database doesn't have 3 columns");
    }

    Topics T;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        int id = std::stoi(row[0]);
        std::string name{row[1]};
        std::string description{row[2]};
        T.emplace_back(id, name, description);
    }
    mysql_free_result(result);
    return T;
}

// Get all the messages for a given topic, but don't store the text
// of the messages.
std::vector<Message> PupperDB::get_message_headers_list(int topic_id)
{
    std::stringstream request;
    request << "SELECT id, sender, recipient, subject, datetime FROM message WHERE topic=" << topic_id << ";";
    int ret = mysql_query(mysql_, request.str().c_str());
    if (ret != 0)
        throw new std::runtime_error(mysql_error(mysql_));

     // The query worked, so let's make a message header list
    MYSQL_RES* result = mysql_store_result(mysql_);
    if (result == nullptr) {
        if (mysql_errno(mysql_) != 0)
            throw new std::runtime_error(mysql_error(mysql_));
        else
            return std::vector<Message>();
    }
   
    if (mysql_num_fields(result) != 5) {
        mysql_free_result(result);
        throw new std::runtime_error("the message header response doesn't have 5 columns");
    }

    std::vector<Message> M;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        int id = std::stoi(row[0]);
        std::string sender{row[1]};
        std::string recipient{row[2]};
        std::string subject{row[3]};
        std::string datestr{row[4]};
        std::vector<std::string> empty_text;
   
        M.emplace_back(id, sender, recipient, topic_id, subject, empty_text);
    }
    mysql_free_result(result);
    return M;    
}

int PupperDB::getAndIncrementCallerCount()
{
    int ret = mysql_query(mysql_, "SELECT callers FROM persistent WHERE id=1;");
    if (ret != 0)
        return -1;
    
    MYSQL_RES *result = mysql_store_result(mysql_);
    if (result == NULL)
        return -1;

    MYSQL_ROW row = mysql_fetch_row(result);
    int callers =  std::stoi(row[0]);
    mysql_free_result(result);

    std::stringstream query;
    query << "UPDATE persistent SET callers=" << std::to_string(callers+1) << " WHERE id=1;";
    mysql_query(mysql_, query.str().c_str());

    return callers;
}


// Prepare a string for inclusion in a query by escaping things
static std::string prep(std::string str)
{
    std::string out = "\"";

    for (char c: str)
    {
        if (c >= ' ' && c <= '~' && c != '\\' && c != '"')
            out += c;
        else {
            out += '\\';
            switch (c) {
            case '\a': out += 'a'; break;
            case '\b': out += 'b'; break;
            case '\t': out += 't'; break;
            case '\n': out += 'n'; break;
            case '\v': out += 'v'; break;
            case '\f': out += 'f'; break;
            case '\r': out += 'r'; break;
            case '\"': out += '"'; break;
            case '\'': out += '\''; break;
            case '\?': out += '?'; break;
            case '\\': out += '\\'; break;

            default:
                out += 'x';
                out += std::string("0123456789ABCDEF")[c >> 4];
                out += std::string("0123456789ABCDEF")[c & 0xF];
            }
        }
    }
    out += '"';
    return out;
}

static std::string prep(std::vector<std::string> vstr)
{
    if (vstr.size() == 0)
        return "\"\"";
    else if (vstr.size() == 1)
        return prep(vstr.front());
    else {
        std::string out;
        for (auto x = vstr.begin(); x != vstr.end() - 1; ++ x) {
            out += *x;
            out += "\n";
        }
        out += vstr.back();
        return prep(out);
    }
}

int PupperDB::insertMessage(std::string& from, std::string& to, std::string& subject,
    int topic_id, std::vector<std::string> text)
{
    std::stringstream request;
    request << "INSERT INTO message (sender,recipient,subject,datetime,topic,message) "
        << "VALUE(" << prep(from) << "," << prep(to) << "," << prep(subject) << ","
        << "NOW(), " << prep(std::to_string(topic_id)) << ", " << prep(text) << ");";

    mysql_query(mysql_, request.str().c_str());
    return 0;
}

#if 0
std::map<int, std::string>& PupperDB::get_message_headers_list(int topic_id)
{
    std::stringstream request;
    MYSQL_RES *result;
    unsigned int num_fields;
    unsigned int num_rows;
    int ret;

    messageList.clear();

    request << "SELECT id, sender, recipient, subject, datetime FROM message WHERE topic=" << std::to_string(topic_id) << ";";
    ret = mysql_query(mysql_, request.str().c_str());
    if (ret == 0) {
        // The query worked, so let's make a message list
        result = mysql_store_result(mysql_);
        if (result)
        {
            num_fields = mysql_num_fields(result);
            
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(result)))
            {
                int id = std::stoi(row[0]);
                std::string sender = row[1];
                std::string recipient = row[2];
                std::string subject = row[3];
                std::string datstr = row[4];
                messageList.insert(std::make_pair(id, subject));
            }
            mysql_free_result(result);
        }
        else {
            // This is an error of some sort
            std::string err = mysql_error(mysql_);
            auto len = err.size();
            if (len > 50) len = 50;
            messageList.insert(make_pair(0, err.substr(0, len)));
        }        
    }
    else {
        // This is an error of some sort
        std::string err = mysql_error(mysql_);
        auto len = err.size();
        if (len > 50) len = 50;
        messageList.insert(make_pair(0, err.substr(0, len)));
    }        

    return messageList;
}
#endif

bool PupperDB::getMessage(int msg_id, string* from, string* to, string* subject, string* date, string* text)
{
    std::stringstream request;
    MYSQL_RES *result;
    unsigned int num_fields;
    unsigned int num_rows;
    int ret;

    request << "SELECT topic, sender, recipient, subject, datetime, message FROM message WHERE id=" << std::to_string(msg_id) << ";";
    ret = mysql_query(mysql_, request.str().c_str());
    if (ret == 0) {
        // The query worked, so let's make a message list
        result = mysql_store_result(mysql_);
        if (result)
        {
            num_fields = mysql_num_fields(result);
            
            MYSQL_ROW row;
            if ((row = mysql_fetch_row(result)))
            {
                int topic = std::stoi(row[0]);
                from->assign(row[1]);
                to->assign(row[2]);
                subject->assign(row[3]);
                date->assign(row[4]);
                text->assign(row[5]);
            }
            mysql_free_result(result);
            return true;
        }
        else {
            from->assign("MYSQL");
            text->assign(mysql_error(mysql_));
            return false;
        }        
    }
    else {
        from->assign("MYSQL");
        text->assign(mysql_error(mysql_));        
        return false;
    }        
}
