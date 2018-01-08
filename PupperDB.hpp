#ifndef PUPPER_DB_HPP
#define PUPPER_DB_HPP

#include <mysql.h>
#include <string>
#undef vector
#include <vector>
#include <map>

using namespace std;

class PupperDB {
private:
    MYSQL *mysql_;
    
public:
    PupperDB(std::string db_password);
    ~PupperDB();
    bool ready;
    std::string errstr;

    int getAndIncrementCallerCount();

    int insertMessage(std::string& from, std::string& to, std::string& subject,
        int topic_id, std::vector<std::string> text);
    std::map<int, std::string>& getMessageList(int topic_id);    
    bool getMessage(int msg_id, string* from, string* to, string* subject, string* date, string* text);

};
#endif