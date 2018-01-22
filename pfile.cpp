#include "pfile.hpp"

Pfile::Pfile()
  : id{-1}
  , name{}
  , description{}
{}

Pfile::Pfile(int _id, const char * _name, const char * _description)
    : id{_id}
    , name{_name}
    , description{_description}
{}

Pfile::Pfile(int& _id, std::string& _name, std::string& _description)
    : id{_id}
    , name{}
    , description{}
{
    name = _name;
    description = _description;
}

