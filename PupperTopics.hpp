#ifndef PUPPER_TOPICS_HPP
#define PUPPER_TOPICS_HPP

#include <string>
#include <vector>

struct PupperTopic {
    std::string name;
    std::string description;
};

typedef std::vector<PupperTopic> PupperTopics; 

int lines(PupperTopics& pt);
int cols(PupperTopics& pt);
#endif