#include <gtest/gtest.h>
#include "../src/Person.h"
#include "../src/PersonEnums.h"

// Basic constructor test using provided constructor code; mostly to ensure CI is working properly
TEST(PersonTests, BasicConstructorTest) {
    Person p("Student123", 2025, Region::US_Northeast, PrimaryOS::Windows, 
             EngineeringFocus::Computer_Systems, StudyTime::Morning, 4);
    EXPECT_EQ(p.getId(), "Student123");
    EXPECT_EQ(p.getGraduationYear(), 2025);
}