#include "PupperDB.hpp"
#include <sstream>
#include <boost/format.hpp>
#include <map>

std::map<int, std::string> messageList;


PupperDB::PupperDB(std::string db_password)
    : ready(false)
    , errstr()
{
    mysql_ = mysql_init(NULL);
    if (mysql_ == NULL) {
        errstr = "mysql_init(): out of memory";
        ready = false;
        return;
    }
    MYSQL *ret = mysql_real_connect(mysql_, NULL, "pupper", db_password.c_str(), "pupper", 0, NULL, 0);
    if (ret == NULL) {
        errstr = mysql_error(mysql_);
        ready = false;
        return;
    }
    ready = true;
}

PupperDB::~PupperDB()
{
    mysql_close(mysql_);
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

std::map<int, std::string>& PupperDB::getMessageList(int topic_id)
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
