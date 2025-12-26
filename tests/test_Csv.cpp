#include <gtest/gtest.h>
#include <fstream>
#include <cstdio> 
#include "../src/CsvPersonReader.h"
#include "../src/Person.h"
#include "../src/PersonEnums.h"
#include "../src/CsvPersonWriter.h"

void createTestFile(const std::string& filename) {
    std::ofstream file(filename);
  
    file << "id,graduationYear,region,primaryOS,engineeringFocus,studyTime,courseLoad,favoriteColors,hobbies,languages\n";

    file << "test1,2025,us-northeast,macos,cybersecurity,night,4,blue,coding,english\n";

    file << "test2,2026,korea,windows,embedded_systems,morning,5,red-green,gaming,korean\n";
    file.close();
}

TEST(CsvReaderTests, ReadsValidCsvFile) {
    std::string testFilename = "temp_test_data.csv";
    createTestFile(testFilename);
    CsvPersonReader reader(testFilename);
    std::vector<Person> people = reader.read();

    ASSERT_EQ(people.size(), 2) << "Should have loaded exactly 2 people from the CSV";

    // Verify Person 1 Details
    Person& p1 = people[0];
    EXPECT_EQ(p1.getId(), "test1");
    EXPECT_EQ(p1.getGraduationYear(), 2025);
    EXPECT_EQ(to_string(p1.getRegion()), "us-northeast");
    EXPECT_EQ(to_string(p1.getPrimaryOS()), "MacOS");
    EXPECT_EQ(to_string(p1.getEngineeringFocus()), "cybersecurity");
    EXPECT_EQ(to_string(p1.getStudyTime()), "Night");
    EXPECT_EQ(p1.getCourseLoad(), 4);

    // verify Person 2 Details
    Person& p2 = people[1];
    EXPECT_EQ(p2.getId(), "test2");
    EXPECT_EQ(to_string(p2.getRegion()), "korea");
    EXPECT_EQ(to_string(p2.getPrimaryOS()), "Windows");

    // delete temproary file
    std::remove(testFilename.c_str());
}

TEST(CsvReaderTests, HandlesMissingFile) {
    // attempting to read a non-existent file
    CsvPersonReader reader("non_existent_file_12345.csv");
    std::vector<Person> people = reader.read();
    EXPECT_TRUE(people.empty()); // should return empty vector
}

TEST(CsvReaderTests, SkipsMalformedLines) {
    std::string filename = "malformed_test.csv";
    std::ofstream file(filename);
    file << "Header\n";
    file << "good_row,2025,us-northeast,macos,cybersecurity,night,4,a,b,c\n";
    file << "bad_row,2025,us-northeast\n"; // missing columns
    file.close();

    CsvPersonReader reader(filename);
    std::vector<Person> people = reader.read();

    EXPECT_EQ(people.size(), 1);
    EXPECT_EQ(people[0].getId(), "good_row");

    // delete temporary file
    std::remove(filename.c_str());
}

TEST(CsvReaderTests, WritesAndReloadsCsvFile) {
    std::string filename = "persist_roundtrip.csv";
    std::vector<Person> originalPeople = {
        Person(
            "alpha",
            2027,
            Region::US_West,
            PrimaryOS::Linux,
            EngineeringFocus::Computer_Systems,
            StudyTime::Afternoon,
            3,
            {"blue", "green"},
            {"coding", "running"},
            {"english", "spanish"}
        ),
        Person(
            "beta",
            2028,
            Region::Japan,
            PrimaryOS::Windows,
            EngineeringFocus::Robotics_CE,
            StudyTime::Morning,
            5,
            {},
            {},
            {"japanese"}
        )
    };

    ASSERT_TRUE(CsvPersonWriter::write(filename, originalPeople));

    CsvPersonReader reader(filename);
    std::vector<Person> loaded = reader.read();

    ASSERT_EQ(loaded.size(), originalPeople.size());

    const Person& p1 = loaded[0];
    EXPECT_EQ(p1.getId(), "alpha");
    EXPECT_EQ(p1.getGraduationYear(), 2027);
    EXPECT_TRUE(p1.getFavoriteColors().count("blue"));
    EXPECT_TRUE(p1.getFavoriteColors().count("green"));
    EXPECT_EQ(p1.getHobbies().size(), 2);
    EXPECT_TRUE(p1.getLanguages().count("english"));
    EXPECT_TRUE(p1.getLanguages().count("spanish"));

    const Person& p2 = loaded[1];
    EXPECT_EQ(p2.getId(), "beta");
    EXPECT_EQ(to_string(p2.getRegion()), "japan");
    EXPECT_TRUE(p2.getLanguages().count("japanese"));

    std::remove(filename.c_str());
}