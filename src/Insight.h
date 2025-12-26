#ifndef INSIGHTS_FINDER_INSIGHT_H
#define INSIGHTS_FINDER_INSIGHT_H

#include <string>
struct Insight {
    std::string text;
    double rating;

    bool operator<(const Insight& other) const {
        return rating < other.rating;
    }
};

#endif //INSIGHTS_FINDER_INSIGHT_H