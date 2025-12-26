#include "MockPersonReader.h"
#include "Person.h"
#include "PersonReader.h"
#include "PersonEnums.h"

std::vector<Person> MockPersonReader::read() {
    std::vector<Person> mockData;

    // Mac users always study at Night
    mockData.push_back(Person("1", 2026, Region::US_Northeast, PrimaryOS::MacOS,
        EngineeringFocus::Cybersecurity, StudyTime::Night, 4,
        {"blue"}, {"gaming"}, {"english"}));

    mockData.push_back(Person("2", 2026, Region::US_Northeast, PrimaryOS::MacOS,
        EngineeringFocus::Cybersecurity, StudyTime::Night, 5,
        {"green"}, {"reading"}, {"english"}));

    mockData.push_back(Person("3", 2027, Region::Korea, PrimaryOS::MacOS,
        EngineeringFocus::Cybersecurity, StudyTime::Night, 3,
        {"red"}, {"coding"}, {"korean"}));

    // Windows users always study in Morning
    mockData.push_back(Person("4", 2026, Region::US_Southwest, PrimaryOS::Windows,
        EngineeringFocus::Embedded_Systems, StudyTime::Morning, 4,
        {"black"}, {"drawing"}, {"english"}));

    mockData.push_back(Person("5", 2027, Region::US_Southwest, PrimaryOS::Windows,
        EngineeringFocus::Embedded_Systems, StudyTime::Morning, 5,
        {"gray"}, {"sports"}, {"spanish"}));

    mockData.push_back(Person("6", 2028, Region::US_Southwest, PrimaryOS::Windows,
        EngineeringFocus::Embedded_Systems, StudyTime::Morning, 3,
        {"white"}, {"music"}, {"english"}));


    // Region EngineeringFocus Correlations

    // Korea & Cybersecurity
    mockData.push_back(Person("7", 2027, Region::Korea, PrimaryOS::Linux,
        EngineeringFocus::Cybersecurity, StudyTime::Night, 4,
        {"purple"}, {"chess"}, {"korean"}));

    mockData.push_back(Person("8", 2028, Region::Korea, PrimaryOS::Linux,
        EngineeringFocus::Cybersecurity, StudyTime::Night, 5,
        {"orange"}, {"strategy-games"}, {"korean"}));

    // US_Northeast & Nueral_Engineering
    mockData.push_back(Person("9", 2026, Region::US_Northeast, PrimaryOS::MacOS,
        EngineeringFocus::Neural_Engineering, StudyTime::Afternoon, 4,
        {"blue"}, {"robotics"}, {"english"}));

    mockData.push_back(Person("10", 2027, Region::US_Northeast, PrimaryOS::MacOS,
        EngineeringFocus::Neural_Engineering, StudyTime::Afternoon, 3,
        {"green"}, {"research"}, {"english"}));

    return mockData;
}