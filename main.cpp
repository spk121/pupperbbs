#include <mysql_devapi.h>
#undef THROW
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <iostream>
#include <ncurses.h>
#include <term.h>
#include <map>
#include <string>
#include <cursesapp.h>
#include "properties.hpp"
#include "PupperApp.hpp"
#undef THROW

using namespace std;
static void write_file_to_cout(const string& fname);
static string query_term_from_cin();
static string query_locale_from_cin();
static void set_the_locale(string locale_str);


int main(int argc, char **argv)
{
    set_the_locale("C");

    // Ini file parsing
    pup_static _ps;
    _ps.load("pupper.xml");

    // ASCII-only opening message.
    write_file_to_cout(_ps.open_msg_filename);

    // Query for a terminal type
    string pup_term = query_term_from_cin();
    if (pup_term.empty())
        return 1;

    // Query for a locale, and then set it.
    string pup_locale_str = query_locale_from_cin();
    if (pup_locale_str.empty())
        return 1;
    set_the_locale(pup_locale_str);

    // Is the database alive?
        // mysqlx::Session sess("localhost", 3306, "pupper", "passw0rd", "pupper"); 
       //  mysqlx::Session sess(mysqlx::SessionOption::USER, "pupper",
       //      mysqlx::SessionOption::PWD, "passw0rd",
       //      mysqlx::SessionOption::DB, "pupper");
    try { 
        mysqlx::Session sess("localhost", 33060, "pupper", _ps.db_password); 
        mysqlx::Schema db = sess.getSchema("pupper");
        mysqlx::Table persist = db.getTable("persistent");
        mysqlx::RowResult rr = persist.select("callers").execute();
        if (rr.count() >= 1)
        {
            auto val = rr.fetchOne();
            cout << "callers " << val[0].getType() << '\n';
            int x = val[0].get<int>();
            
        }
        else
        {
            cout << "no callers value found in persistent table\n";
        }
        // auto count = persist.find("callers").limit(1).execute();
    } catch (const mysqlx::Error& e)
    {
        cerr << e.what();
        return 1;
    }
    
    PupperApp A;
    A.handleArgs(argc, argv);
    endwin();

    A();   // This is ncurses++ weird shorthand for 'go'.
    usleep(1000000);
    endwin();
#if 0    
    if (!A)
        return 1;
    else {
        int res;
        A->handleArgs(argc, argv);
        ::endwin();
        try {
            res = (*A)();
            ::endwin();
        }
        catch(const NCursesException &e) {
            ::endwin();
            cerr << e.message << endl;
            res = e.errorno;
        }

        delete A;
        //w_nc_free_and_exit(res);
        return res;
    }
#endif
    // tigetflag, tigetnum, tigetstr
    
    // Memory allocation of message catalog

    // Enough memory checking

    // Install clock thread
    // Init hardware
    // - scrinit
    // - serial port open

    // set up messsage flags: event, ready, modem

    // Admin-level menu
    // - keyhit
    // - scheduler
    // - wake up modem if modem flag says to 
    // - write top level menu if ready flag says to

    // Login
}

static void write_file_to_cout(const string& fname)
{
    // ASCII opening message.
    auto msgfile = ifstream(fname);
    if (msgfile.good())
    {
        string line;
        while (getline(msgfile, line))
        {
            if (line.empty() || line.front() != '#')
                cout << line << "\n";
        }
    }
}

static string query_term_from_cin()
{
    string term;
    cout << "\n";
    cout << "Enter your terminal type, or just hit ENTER for 'xterm-256color'.\n";
    cout << "Term: ";
    getline(std::cin, term);
    boost::algorithm::trim(term);
    if (term.empty()) term = "xterm-256color";

    int ret, erret;
    ret = setupterm(term.c_str(), 0, &erret); 
    if (ret == ERR)
    {
        if (erret)
            cout << "The terminal '" << longname() << "' lacks necessary functionality.\n";
        else
            cout << "The terminal '" << term << "' is unknown.\n";
        return "";
    }

    cout << "The terminal is a " << longname() << ".\n";
    return term;
}

const map<unsigned, string> locale_map = {
    {1, "UTF-8"},
    {2, "Latin 1     ISO-8859-1     Windows-1252"},
    {3, "ASCII"},
    {4, "Latin 9     ISO-8859-15    Windows-28605"},
};

static string query_locale_from_cin()
{
    string lcl;

    cout << "\n";
    cout << "Enter the # of the appropriate locale, or just hit ENTER for 'UTF-8'.\n";
    for (auto it = locale_map.begin(); it != locale_map.end(); ++it)
        cout << it->first << ". " << it->second << '\n';
    cout << "Locale #: ";
    getline(std::cin, lcl);
    boost::algorithm::trim(lcl);

    try {
        if (lcl.empty())
            lcl = "en_US.utf8";
        else 
        {
            int n = stoi(lcl);     
            if (n == 1)
                lcl = "en_US.utf8";
            else if (n == 2)
                lcl = "en_US.iso88591";
            else if (n == 3)
                lcl = "C";
            else if (n == 4)
                lcl = "en_US.iso885915";
        }
    } catch (...) {
        lcl = "C";
    }
    cout << "Your locale is set to  " << lcl << '\n';
    return lcl;
}

static void set_the_locale(string locale_str)
{
    locale pup_locale = locale(locale_str);
    locale::global(pup_locale);

    cout.sync_with_stdio(false);
    cin.sync_with_stdio(false);
    cerr.sync_with_stdio(false);

    wcout.sync_with_stdio(false);
    wcin.sync_with_stdio(false);
    wcerr.sync_with_stdio(false);

    cout.imbue(pup_locale);
    cin.imbue(pup_locale);
    cerr.imbue(pup_locale);

    wcout.imbue(pup_locale);
    wcin.imbue(pup_locale);
    wcerr.imbue(pup_locale);
}
