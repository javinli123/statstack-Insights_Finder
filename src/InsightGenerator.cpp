#include "InsightGenerator.h"
#include "PersonEnums.h"
#include <map>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

// Helper function: converts an unordered set to a string vector
// Not included in header because it is not to be used in any other files
static std::vector<std::string> setToVector(const std::unordered_set<std::string>& s) {
    return std::vector<std::string>(s.begin(), s.end());
}

void InsightGenerator::analyzeOverlap(
    const std::vector<Person> &people,
    std::vector<Insight> &insights,
    std::string traitA,
    std::string traitB,
    std::function<std::vector<std::string>(const Person&)> getTraitA,
    std::function<std::vector<std::string>(const Person&)> getTraitB
) {
    std::map<std::string, int> countA;
    std::map<std::string, int> countOverlap;

    for (const auto &person : people) {
        std::vector<std::string> valsA = getTraitA(person);
        std::vector<std::string> valsB = getTraitB(person);

        // Filter out unknowns from insight generation
        for (const auto& valA : valsA) {
            if (valA == "Unknown" || valA == "unknown") {
                continue;
            }
            countA[valA]++;
            for (const auto& valB : valsB) {
                if (valB == "Unknown" || valB == "unknown") {
                    continue;
                }
                countOverlap[valA + "|" + valB]++;
            }
        }
    }

    for (auto const &[keyOverlap, numOverlap] : countOverlap) {
        size_t delimiterIndex = keyOverlap.find("|");
        std::string valA = keyOverlap.substr(0, delimiterIndex);
        std::string valB = keyOverlap.substr(delimiterIndex + 1);
        int totalInGroupA = countA[valA];

        // skip any insights considering less than 5 people
        if (totalInGroupA <= 5) {
            continue;
        }

        double rating = (static_cast<double>(numOverlap) / totalInGroupA) * 100.0;

        // skip any insights with a rating under 20%
        if (rating < 20.0) {
            continue;
        }

        Insight in;
        in.text = "People whose " + traitA + " is " + valA +
            " tend to have " + traitB + " of " + valB + ".";
        in.rating = rating;

        insights.push_back(in);
    }
}

std::vector<Insight> InsightGenerator::generateInsights(const std::vector<Person> &people) {
    std::vector<Insight> insights;

    // PRIMARY OS VS STUDY TIME
    analyzeOverlap(people, insights, "primary OS", "study time",
        [](const Person& p) {return std::vector<std::string>{to_string(p.getPrimaryOS())};},
        [](const Person& p) {return std::vector<std::string>{to_string(p.getStudyTime())};}
        );
    // PRIMARY OS VS HOBBY
    analyzeOverlap(people, insights, "primary OS", "hobby",
        [](const Person& p) {return std::vector<std::string>{to_string(p.getPrimaryOS())};},
        [](const Person& p) {return setToVector(p.getHobbies());}
        );
    // REGION VS STUDY TIME
    analyzeOverlap(people, insights, "region", "study time",
            [](const Person& p) {return std::vector<std::string>{to_string(p.getRegion())};},
            [](const Person& p) {return std::vector<std::string>{to_string(p.getStudyTime())};}
            );
    // REGION VS ENGINEERING FOCUS
    analyzeOverlap(people, insights, "region", "engineering focus",
        [](const Person& p) {return std::vector<std::string>{to_string(p.getRegion())};},
        [](const Person& p) {return std::vector<std::string>{to_string(p.getEngineeringFocus())};}
        );
    // REGION VS FAVORITE COLOR
    analyzeOverlap(people, insights, "region", "favorite color",
            [](const Person& p) {return std::vector<std::string>{to_string(p.getRegion())};},
            [](const Person& p) {return setToVector(p.getFavoriteColors());}
            );
    // ENGINEERING FOCUS VS HOBBY
    analyzeOverlap(people, insights, "engineering focus", "hobby",
        [](const Person& p) {return std::vector<std::string>{to_string(p.getEngineeringFocus())};},
        [](const Person& p) {return setToVector(p.getHobbies());}
    );
    // ENGINEERING FOCUS VS FAVORITE COLOR
    analyzeOverlap(people, insights, "engineering focus", "favorite color",
        [](const Person& p) {return std::vector<std::string>{to_string(p.getEngineeringFocus())};},
        [](const Person& p) {return setToVector(p.getFavoriteColors());}
    );

    std::sort(insights.rbegin(), insights.rend());
    return insights;
}