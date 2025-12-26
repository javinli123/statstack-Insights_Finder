#ifndef INSIGHTSAPP_H
#define INSIGHTSAPP_H

#include <memory>
#include <vector>
#include <unordered_set>
#include "PersonReader.h"
#include "Person.h"

class InsightsApp {
private:
    std::unique_ptr<PersonReader> reader_;
    std::vector<Person> people_;

    void promptDatasetUpdates();
    void printDataset() const;
    Person promptForPerson(const Person* existing = nullptr);

    std::string promptString(const std::string& label, const std::string& defaultValue = "");
    int promptInt(const std::string& label, int defaultValue = 0);
    Region promptRegion(Region defaultValue = Region::Unknown);
    PrimaryOS promptPrimaryOS(PrimaryOS defaultValue = PrimaryOS::Unknown);
    EngineeringFocus promptEngineeringFocus(EngineeringFocus defaultValue = EngineeringFocus::Unknown);
    StudyTime promptStudyTime(StudyTime defaultValue = StudyTime::Unknown);
    std::unordered_set<std::string> promptTagSet(const std::string& label, const std::unordered_set<std::string>& defaults = {});
public:
    explicit InsightsApp(std::unique_ptr<PersonReader> reader);
    void run();
};

#endif //INSIGHTSAPP_H