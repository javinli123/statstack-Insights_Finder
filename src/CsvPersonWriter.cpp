#include "CsvPersonWriter.h"
#include "PersonEnums.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace {
std::string joinSet(const std::unordered_set<std::string>& items) {
    std::ostringstream oss;
    bool first = true;
    for (const auto& item : items) {
        if (!first) {
            oss << ';';
        }
        oss << item;
        first = false;
    }
    return oss.str();
}
}

bool CsvPersonWriter::write(const std::string& filename, const std::vector<Person>& people) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file for write: " << filename << std::endl;
        return false;
    }

    file << "id,graduationYear,region,primaryOS,engineeringFocus,studyTime,courseLoad,favoriteColors,hobbies,languages\n";

    for (const auto& person : people) {
        file << person.getId() << ','
             << person.getGraduationYear() << ','
             << to_string(person.getRegion()) << ','
             << to_string(person.getPrimaryOS()) << ','
             << to_string(person.getEngineeringFocus()) << ','
             << to_string(person.getStudyTime()) << ','
             << person.getCourseLoad() << ','
             << joinSet(person.getFavoriteColors()) << ','
             << joinSet(person.getHobbies()) << ','
             << joinSet(person.getLanguages()) << '\n';
    }

    return true;
}