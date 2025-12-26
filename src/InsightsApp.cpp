#include "InsightsApp.h"
#include "CsvPersonReader.h"
#include "CsvPersonWriter.h"
#include "HtmlReportWriter.h"
#include "InsightGenerator.h"
#include "PersonCollectionUtils.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <sstream>

InsightsApp::InsightsApp(std::unique_ptr<PersonReader> reader)
    : reader_(std::move(reader)) {}

void InsightsApp::run() {
    std::cout << "InsightApp: Run" << std::endl;
    people_ = reader_->read();
    std::cout << "InsightApp: Loaded " << people_.size() << " people." << std::endl;

    promptDatasetUpdates();

    if (people_.empty()) {
        std::cout << "InsightApp: No people loaded. Analysis cannot be completed." << std::endl;
        return;
    }

    InsightGenerator generator;
    std::cout << "InsightApp: Analyzing..." << std::endl;
    std::vector<Insight> insights = generator.generateInsights(people_);

    std::string persistTarget = "persisted_people.csv";
    if (auto csvReader = dynamic_cast<CsvPersonReader*>(reader_.get())) {
        persistTarget = csvReader->getFilename();
    }

    if (CsvPersonWriter::write(persistTarget, people_)) {
        std::cout << "InsightApp: Saved current dataset to " << persistTarget << std::endl;
    }

    std::cout << "-== RESULTS ==-" << std::endl;
    if (insights.empty()) {
        std::cout << "No insights found." << std::endl;
    } else {
        for (const auto &item : insights) {
            std::cout << "(" << item.rating << "%) " << item.text << std::endl;
        }
    }

    // HTML Report Writer
    if (!insights.empty()) {
        HtmlReportWriter::writeReport("report.html", insights);
    }
}

void InsightsApp::promptDatasetUpdates() {
    // Clear any leftover newline from earlier formatted input operations.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true) {
        std::cout << "\nDataset editor:" << std::endl;
        std::cout << "1. Add new person" << std::endl;
        std::cout << "2. Modify existing person" << std::endl;
        std::cout << "3. View dataset" << std::endl;
        std::cout << "0. Continue to analysis" << std::endl;
        std::cout << "> ";

        std::string choiceLine;
        if (!std::getline(std::cin, choiceLine)) {
            break;
        }

        std::stringstream choiceStream(choiceLine);
        int choice = 0;
        choiceStream >> choice;

        if (choice == 0) {
            break;
        }

        if (choice == 1) {
            Person newPerson = promptForPerson();
            if (PersonCollectionUtils::addPersonIfMissing(people_, newPerson)) {

                std::cout << "Added person with id '" << newPerson.getId() << "'." << std::endl;

            } else {

                std::cout << "A person with id '" << newPerson.getId() << "' already exists. Use modify instead." << std::endl;

            }
        } else if (choice == 2) {
            std::cout << "Enter id to modify > ";
            std::string id;
            if (!std::getline(std::cin, id)) {
                break;
            }

            auto existing = std::find_if(people_.begin(), people_.end(), [&](const Person& p) {
                return p.getId() == id;
            });

            if (existing == people_.end()) {
                std::cout << "No person found with id '" << id << "'." << std::endl;
                continue;
            }

            Person updated = promptForPerson(&(*existing));
            PersonCollectionUtils::updatePersonById(people_, id, updated);
            std::cout << "Updated person with id '" << id << "'." << std::endl;
        } else if (choice == 3) {
            printDataset();
        } else {
            std::cout << "Unrecognized option. Try again." << std::endl;
        }
    }
}

void InsightsApp::printDataset() const {
    if (people_.empty()) {
        std::cout << "Dataset is empty." << std::endl;
        return;
    }

    std::cout << "\n-- Current Dataset --" << std::endl;
    for (const auto& person : people_) {
        std::cout << person.toString() << std::endl;
    }
}

std::string InsightsApp::promptString(const std::string& label, const std::string& defaultValue) {
    std::cout << label;
    if (!defaultValue.empty()) {
        std::cout << " [" << defaultValue << "]";
    }
    std::cout << " > ";

    std::string line;
    std::getline(std::cin, line);
    if (line.empty()) {
        return defaultValue;
    }
    return line;
}

int InsightsApp::promptInt(const std::string& label, int defaultValue) {
    while (true) {
        std::string value = promptString(label, defaultValue == 0 ? std::string() : std::to_string(defaultValue));
        if (value.empty()) {
            return defaultValue;
        }

        std::stringstream stream(value);
        int result;
        if (stream >> result) {
            return result;
        }

        std::cout << "Invalid number. Please try again." << std::endl;
    }
}

std::unordered_set<std::string> InsightsApp::promptTagSet(const std::string& label, const std::unordered_set<std::string>& defaults) {
    std::string defaultValue;
    for (const auto& item : defaults) {
        if (!defaultValue.empty()) {
            defaultValue += ';';
        }
        defaultValue += item;
    }

    std::string raw = promptString(label, defaultValue);
    if (raw.empty()) {
        return defaults;
    }

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

Region InsightsApp::promptRegion(Region defaultValue) {
    const auto options = all_region_strings();
    while (true) {
        std::cout << "Region options: ";
        for (const auto& opt : options) {
            std::cout << opt << ' ';
        }
        std::cout << std::endl;

        std::string chosen = promptString("Region", to_string(defaultValue));
        Region parsed = parse_region(chosen);
        if (parsed != Region::Unknown || chosen == to_string(defaultValue)) {
            return parsed;
        }

        std::cout << "Invalid region. Please select from the list." << std::endl;
    }
}

PrimaryOS InsightsApp::promptPrimaryOS(PrimaryOS defaultValue) {
    const auto options = all_primary_os_strings();
    while (true) {
        std::cout << "Primary OS options: ";
        for (const auto& opt : options) {
            std::cout << opt << ' ';
        }
        std::cout << std::endl;

        std::string chosen = promptString("Primary OS", to_string(defaultValue));
        PrimaryOS parsed = parse_primary_os(chosen);
        if (parsed != PrimaryOS::Unknown || chosen == to_string(defaultValue)) {
            return parsed;
        }

        std::cout << "Invalid OS. Please select from the list." << std::endl;
    }
}

EngineeringFocus InsightsApp::promptEngineeringFocus(EngineeringFocus defaultValue) {
    const auto options = all_engineering_focus_strings();
    while (true) {
        std::cout << "Engineering focus options: ";
        for (const auto& opt : options) {
            std::cout << opt << ' ';
        }
        std::cout << std::endl;

        std::string chosen = promptString("Engineering Focus", to_string(defaultValue));
        EngineeringFocus parsed = parse_engineering_focus(chosen);
        if (parsed != EngineeringFocus::Unknown || chosen == to_string(defaultValue)) {
            return parsed;
        }

        std::cout << "Invalid focus. Please select from the list." << std::endl;
    }
}

StudyTime InsightsApp::promptStudyTime(StudyTime defaultValue) {
    const auto options = all_study_time_strings();
    while (true) {
        std::cout << "Study time options: ";
        for (const auto& opt : options) {
            std::cout << opt << ' ';
        }
        std::cout << std::endl;

        std::string chosen = promptString("Study Time", to_string(defaultValue));
        StudyTime parsed = parse_study_time(chosen);
        if (parsed != StudyTime::Unknown || chosen == to_string(defaultValue)) {
            return parsed;
        }

        std::cout << "Invalid study time. Please select from the list." << std::endl;
    }
}

Person InsightsApp::promptForPerson(const Person* existing) {
    std::string id = promptString("ID", existing ? existing->getId() : std::string());
    int graduationYear = promptInt("Graduation Year", existing ? existing->getGraduationYear() : 0);
    Region region = promptRegion(existing ? existing->getRegion() : Region::Unknown);
    PrimaryOS os = promptPrimaryOS(existing ? existing->getPrimaryOS() : PrimaryOS::Unknown);
    EngineeringFocus focus = promptEngineeringFocus(existing ? existing->getEngineeringFocus() : EngineeringFocus::Unknown);
    StudyTime studyTime = promptStudyTime(existing ? existing->getStudyTime() : StudyTime::Unknown);
    int courseLoad = promptInt("Course Load", existing ? existing->getCourseLoad() : 0);
    auto colors = promptTagSet("Favorite Colors (semicolon separated)", existing ? existing->getFavoriteColors() : std::unordered_set<std::string>{});
    auto hobbies = promptTagSet("Hobbies (semicolon separated)", existing ? existing->getHobbies() : std::unordered_set<std::string>{});
    auto languages = promptTagSet("Languages (semicolon separated)", existing ? existing->getLanguages() : std::unordered_set<std::string>{});

    return Person(id, graduationYear, region, os, focus, studyTime, courseLoad, colors, hobbies, languages);
}