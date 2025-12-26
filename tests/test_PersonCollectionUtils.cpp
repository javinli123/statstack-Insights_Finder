#include <gtest/gtest.h>
#include "../src/PersonCollectionUtils.h"

TEST(PersonCollectionUtilsTests, AddsWhenIdIsUnique) {
    std::vector<Person> people;
    Person alice("alice", 2026, Region::UK_Ireland, PrimaryOS::MacOS, EngineeringFocus::Cybersecurity, StudyTime::Morning, 4);

    EXPECT_TRUE(PersonCollectionUtils::addPersonIfMissing(people, alice));
    ASSERT_EQ(people.size(), 1);
    EXPECT_EQ(people[0].getId(), "alice");
}

TEST(PersonCollectionUtilsTests, PreventsDuplicateIds) {
    std::vector<Person> people = {
        Person("sam", 2025, Region::US_West, PrimaryOS::Linux, EngineeringFocus::Networking, StudyTime::Night, 3)
    };

    Person duplicate("sam", 2027, Region::Japan, PrimaryOS::Windows, EngineeringFocus::Robotics_CE, StudyTime::Afternoon, 5);
    EXPECT_FALSE(PersonCollectionUtils::addPersonIfMissing(people, duplicate));
    ASSERT_EQ(people.size(), 1);
    EXPECT_EQ(people[0].getGraduationYear(), 2025);
}

TEST(PersonCollectionUtilsTests, UpdatesExistingPersonById) {
    std::vector<Person> people = {
        Person("lee", 2024, Region::China, PrimaryOS::Windows, EngineeringFocus::Manufacturing, StudyTime::Morning, 4)
    };

    Person updated("lee", 2025, Region::Japan, PrimaryOS::MacOS, EngineeringFocus::Computer_Systems, StudyTime::Night, 6);
    EXPECT_TRUE(PersonCollectionUtils::updatePersonById(people, "lee", updated));
    ASSERT_EQ(people.size(), 1);
    EXPECT_EQ(people[0].getGraduationYear(), 2025);
    EXPECT_EQ(to_string(people[0].getRegion()), "japan");
}

TEST(PersonCollectionUtilsTests, UpdateFailsWhenIdNotFound) {
    std::vector<Person> people = {
        Person("pat", 2023, Region::Australia, PrimaryOS::Linux, EngineeringFocus::Energy_Systems, StudyTime::Afternoon, 2)
    };

    Person updated("alex", 2028, Region::Canada_West, PrimaryOS::MacOS, EngineeringFocus::Fluid_Mechanics, StudyTime::Morning, 5);
    EXPECT_FALSE(PersonCollectionUtils::updatePersonById(people, "alex", updated));
    EXPECT_EQ(people[0].getId(), "pat");
}

TEST(PersonCollectionUtilsTests, UpdateFailsWhenChangingIdToDuplicate) {
    std::vector<Person> people = {
        Person("primary", 2026, Region::UK_Ireland, PrimaryOS::MacOS, EngineeringFocus::Cybersecurity, StudyTime::Night, 4),
        Person("secondary", 2027, Region::Japan, PrimaryOS::Windows, EngineeringFocus::Networking, StudyTime::Morning, 5)
    };

    Person updated("secondary", 2026, Region::Canada_East, PrimaryOS::Linux, EngineeringFocus::Control_Systems, StudyTime::Night, 6);
    EXPECT_FALSE(PersonCollectionUtils::updatePersonById(people, "primary", updated));
    EXPECT_EQ(people[0].getId(), "primary");
    EXPECT_EQ(to_string(people[0].getRegion()), "uk-ireland");
}