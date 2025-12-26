#include "RandomPersonReader.h"
#include "Person.h"
#include "PersonReader.h"
#include "PersonEnums.h"

#include <iostream>

RandomPersonReader::RandomPersonReader(int count)
    : n(count) {}

Person RandomPersonReader::randomPerson() {
    static int nextId = 1;
    std::string id = "random_" + std::to_string(nextId++);

    int graduationYear = 2000 + (std::rand() % 30);

    Region region = static_cast<Region>(
        std::rand() % (static_cast<int>(Region::Unknown) + 1)
    );
    PrimaryOS primaryOS = static_cast<PrimaryOS>(
        std::rand() % (static_cast<int>(PrimaryOS::Unknown) + 1)
    );
    EngineeringFocus engineeringFocus = static_cast<EngineeringFocus>(
        std::rand() % (static_cast<int>(EngineeringFocus::Unknown) + 1)
    );
    StudyTime studyTime = static_cast<StudyTime>(
        std::rand() % (static_cast<int>(StudyTime::Unknown) + 1)
    );

    int courseLoad = 1 + (std::rand() % 6);

    std::vector<std::string> colorPool = {
        "red", "blue", "green", "purple", "black", "white"
    };
    std::vector<std::string> hobbyPool = {
        "reading", "gaming", "sports", "music", "coding", "cooking"
    };
    std::vector<std::string> languagePool = {
        "English", "Spanish", "Chinese", "French", "Korean", "Hindi"
    };

    std::unordered_set<std::string> favoriteColors = {
        colorPool[std::rand() % colorPool.size()]
    };
    std::unordered_set<std::string> hobbies = {
        hobbyPool[std::rand() % hobbyPool.size()]
    };
    std::unordered_set<std::string> languages = {
        languagePool[std::rand() % languagePool.size()]
    };

    return Person(
        id,
        graduationYear,
        region,
        primaryOS,
        engineeringFocus,
        studyTime,
        courseLoad,
        favoriteColors,
        hobbies,
        languages
    );
}

std::vector<Person> RandomPersonReader::read() {
    std::cout << "Generating " << n << " random people...\n";

    std::vector<Person> randomPersons;
    randomPersons.reserve(n);
    for (int i = 0; i < n; i++) {
        randomPersons.push_back(randomPerson());
    }
    return randomPersons;
}