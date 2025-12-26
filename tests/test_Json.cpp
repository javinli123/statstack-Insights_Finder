#include "gtest/gtest.h"
#include "JsonPersonReader.h"
#include "PersonEnums.h"
#include <unordered_set>
#include <string>

// URL points to your test JSON
const std::string TEST_URL = "http://gist.githubusercontent.com/esolovey-bu/cba6c1b4eedd0a621ce879e6e6299d28/raw/sample_people.json?v=2";

TEST(JsonPersonReaderTest, LoadsPeopleFromHttp) {
    JsonPersonReader reader(TEST_URL);
    auto people = reader.read();

    // 1. Basic checks
    ASSERT_FALSE(people.empty()) << "JSON did not load or parse correctly.";
    ASSERT_GE(people.size(), 3) << "Expected at least 3 sample people.";

    // 2. Check first person
    const Person& p = people[0];

    EXPECT_FALSE(p.getId().empty());
    EXPECT_GT(p.getGraduationYear(), 2000);

    // 3. Tag sets are parsed either from arrays or dashed strings
    EXPECT_FALSE(p.getFavoriteColors().empty());
    EXPECT_FALSE(p.getHobbies().empty());
    EXPECT_FALSE(p.getLanguages().empty());
}

TEST(JsonPersonReaderTest, AllPeopleHaveValidFields) {
    JsonPersonReader reader(TEST_URL);
    auto people = reader.read();

    ASSERT_FALSE(people.empty()) << "No people were read from the JSON";

    int valid0sCount = 0;

    for (const auto& p : people) {
        // IDs and Years should always be valid
        EXPECT_FALSE(p.getId().empty());
        EXPECT_GT(p.getGraduationYear(), 1900);

        // Count how many people have valid OSes, but DO NOT fail if few are unknown
        // For example, TempleOS will parse as Unknown, that doesn't mean broken parser
        if (p.getPrimaryOS() != PrimaryOS::Unknown) {
            valid0sCount++;
        }

        // Sets can be empty and shouldn't crash
        EXPECT_GE(p.getFavoriteColors().size(), 0);
        EXPECT_GE(p.getHobbies().size(), 0);
        EXPECT_GE(p.getLanguages().size(), 0);
    }

    EXPECT_GT(valid0sCount, people.size() / 2) << "Too many unknown OSes; parser may be broken";
}

TEST(JsonPersonReaderTest, InvalidURLReturnsEmpty) {
    JsonPersonReader reader("http://invalid-url");
    auto people = reader.read();
    EXPECT_TRUE(people.empty()) << "Reader should return empty vector for bad URLs";
}