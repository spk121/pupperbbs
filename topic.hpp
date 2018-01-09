#ifndef PUPPER_TOPICS_HPP
#define PUPPER_TOPICS_HPP

#include <string>
#include <vector>

struct Topic {
    //Topic(int _id, std::string _name, std::string _description);
    Topic(int& _id, std::string& _name, std::string& _description);

    int id;
    std::string name;
    std::string description;
};

typedef std::vector<Topic> Topics; 

std::size_t topics_line_count(Topics& pt);
std::size_t topics_col_count(Topics& pt);
#endif