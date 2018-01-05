#ifndef PUPPER_DB_HPP
#define PUPPER_DB_HPP

#undef THROW
#include <mysql_devapi.h>
#undef THROW
#include <string>
#undef THROW

class PupperDB {
private:
    mysqlx::Session* psession_;
    mysqlx::Schema*  pschema_;
    
public:
    PupperDB(std::string db_password);
    bool ready;
    std::string errstr;

    int getAndIncrementCallerCount();
};
#endif