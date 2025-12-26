#include "CsvPersonReader.h"
#include "PersonEnums.h" // for parse getters
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <unordered_set>

static std::vector<std::string> splitCsvLine(const std::string& line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string item;

    // Split the line at every comma
    while (std::getline(ss, item, ',')) {
        result.push_back(item);
    }
    return result;
}

std::unordered_set<std::string> CsvPersonReader::parseSet(const std::string& raw) {
    std::unordered_set<std::string> values;
    std::stringstream ss(raw);
    std::string item;

    while (std::getline(ss, item, ';')) {
        item.erase(item.begin(), std::find_if(item.begin(), item.end(), [](unsigned char c) {
            return !std::isspace(c);
        }));
        item.erase(std::find_if(item.rbegin(), item.rend(), [](unsigned char c) {
            return !std::isspace(c);
        }).base(), item.end());

        if (!item.empty()) {
            values.insert(item);
        }
    }

    return values;
}

CsvPersonReader::CsvPersonReader(const std::string& filename)
    : filename_(filename) {}

std::vector<Person> CsvPersonReader::read() {
    std::vector<Person> people;
    std::ifstream file(filename_);

    if (!file.is_open()) {
        std::cout << "Error opening file " << filename_ << std::endl;
        return people;
    }

    std::string line;

    // Skip header line; if the CSV does not have a header line, it will skip first entry
    std::getline(file, line);

    while (std::getline(file, line)) {
        // Consider and skip empty lines
        if (line.empty()) continue;

        // Break line into columns
        std::vector<std::string> parts = splitCsvLine(line);

        // Person takes 10 arguments, so skip lines w/o 10 args (error lines)
        if (parts.size() != 10) continue;

        std::string id = parts[0];
        int gradYear = std::stoi(parts[1]);
        Region region = parse_region(parts[2]);
        PrimaryOS os = parse_primary_os(parts[3]);
        EngineeringFocus focus = parse_engineering_focus(parts[4]);
        StudyTime time = parse_study_time(parts[5]);
        int load = std::stoi(parts[6]);

        std::unordered_set<std::string> colors = parseSet(parts[7]);
        std::unordered_set<std::string> hobbies = parseSet(parts[8]);
        std::unordered_set<std::string> languages = parseSet(parts[9]);

        Person p(id, gradYear, region, os, focus, time, load, colors, hobbies, languages);
        people.push_back(p);
    }
    std::cout << "Successfully loaded " << people.size() << " people from " << filename_ << " (CSV)." << std::endl;
    return people;
}