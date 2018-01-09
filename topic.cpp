#include "topic.hpp"

#if 0
Topic::Topic(int _id, std::string _name, std::string _description)
    : id{_id}
    , name{_name}
    , description{_description}
{}
#endif

Topic::Topic(int& _id, std::string& _name, std::string& _description)
    : id{_id}
    , name{}
    , description{}
{
    name = _name;
    description = _description;
}

std::size_t topics_line_count(Topics& pt) {
    return pt.size(); 
}

std::size_t topics_col_count(Topics& pt) {
    std::size_t width1 = 0, width2 = 0;
    for (auto& t: pt)
    {
        if (t.name.size() > width1)
            width1 = t.name.size();
        if (t.description.size() > width2)
            width2 = t.description.size();
    }
    // Add 1 for the space between columns
    return width1 + width2 + 1;
}
