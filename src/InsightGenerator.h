#ifndef INSIGHTGENERATOR_H
#define INSIGHTGENERATOR_H

#include "Person.h"
#include "Insight.h"
#include <vector>
#include <functional>

class InsightGenerator {
public:
    std::vector<Insight> generateInsights(
        const std::vector<Person>& people
        );
private:
    void analyzeOverlap(
        const std::vector<Person> &people,
        std::vector<Insight> &insights,
        std::string traitA,
        std::string traitB,
        std::function<std::vector<std::string>(const Person&)> getTraitA,
        std::function<std::vector<std::string>(const Person&)> getTraitB
        );
};

#endif //INSIGHTGENERATOR_H