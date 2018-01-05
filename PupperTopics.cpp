#include "PupperTopics.hpp"

int lines(PupperTopics& pt) { return pt.size(); }

int cols(PupperTopics& pt) {
    int width1 = 0, width2 = 0;
    for (auto& t: pt)
    {
        if (t.name.size() > width1)
            width1 = t.name.size();
        if (t.description.size() > width2)
            width2 = t.description.size();
    }
    return width1 + width2 + 1;
}
