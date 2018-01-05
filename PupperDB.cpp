#undef THROW
#include "PupperDB.hpp"
#undef THROW
#include "cursesw.h"
#undef THROW


PupperDB::PupperDB(std::string db_password)
    : psession_(nullptr)
    , pschema_(nullptr)
    , ready(false)
    , errstr()
{
    try {
        psession_ = new mysqlx::Session("localhost", 33060, "pupper", db_password, "pupper");
        ready = true;
    }
    catch (const mysqlx::Error& e) {
        ready = false;
        errstr = e.what();
    }
}

int PupperDB::getAndIncrementCallerCount()
{
    int callers;

    mysqlx::Schema schema = psession_->getSchema("pupper");
    mysqlx::Table p = schema.getTable("persistent");
    mysqlx::RowResult rr = p.select("callers").where("id=1").execute();
    mysqlx::Row r = rr.fetchOne();
    callers = r[0].get<int>();
    p.update().set("callers", callers + 1).where("id=1").execute();

    return callers;
}

int PupperDB::insertMessage(std::string& from, std::string& to, std::string& subject,
    int topic_id, std::vector<std::string> text)
{

}