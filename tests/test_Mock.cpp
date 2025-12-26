#include <gtest/gtest.h>
#include "../src/MockPersonReader.h"
#include "../src/InsightGenerator.h"
#include "../src/Person.h"
#include <vector>
#include <string>
#include <iostream>

// # Verify the Mock Reader loads the correct rigged data
TEST(MockReaderTests, LoadsRiggedData) {
    MockPersonReader reader;
    std::vector<Person> people = reader.read();

    // 10 people were added in MockPersonReader.cpp
    ASSERT_EQ(people.size(), 10);

    EXPECT_EQ(to_string(people[0].getPrimaryOS()), "MacOS");
    EXPECT_EQ(to_string(people[0].getStudyTime()), "Night");

    EXPECT_EQ(to_string(people[3].getPrimaryOS()), "Windows");
    EXPECT_EQ(to_string(people[3].getStudyTime()), "Morning");
}

// # Verify the Generator finds the 100% Windows-Morning Correlation
TEST(InsightGeneratorTests, FindsWindowsMorningCorrelation) {
    MockPersonReader reader;
    std::vector<Person> people = reader.read();
    InsightGenerator generator;

    std::vector<Insight> insights = generator.generateInsights(people);

    bool foundWindowsTrend = false;
    for (const auto& insight : insights) {
        // "People whose primary OS is Windows tend to have study time of Morning."
        if (insight.text.find("Windows") != std::string::npos &&
            insight.text.find("Morning") != std::string::npos) {
            foundWindowsTrend = true;
            EXPECT_EQ(insight.rating, 100.0);
        }
    }
    EXPECT_TRUE(foundWindowsTrend) << "Windows-Morning correlation not found";
}

// # Verify the Generator finds the Korea-Cybersecurity Correlation
TEST(InsightGeneratorTests, FindsKoreaCybersecurityCorrelation) {
    MockPersonReader reader;
    std::vector<Person> people = reader.read();
    InsightGenerator generator;

    std::vector<Insight> insights = generator.generateInsights(people);

    bool foundKoreaTrend = false;
    for (const auto& insight : insights) {
        if (insight.text.find("korea") != std::string::npos &&
            insight.text.find("cybersecurity") != std::string::npos) {
            foundKoreaTrend = true;
            EXPECT_EQ(insight.rating, 100.0);
        }
    }
    EXPECT_TRUE(foundKoreaTrend) << "Korea-Cybersecurity correlation not found";

}