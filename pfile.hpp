#ifndef PUPPER_PFILE_H
#define PUPPER_PFILE_H

#include <string>

struct Pfile {
  Pfile();
  Pfile(int _id, const char * _name, const char * _description);
  Pfile(int& _id, std::string& _name, std::string& _description);
  
  int id;
  std::string name;
  std::string description;
};



#endif
