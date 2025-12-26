#include "PersonCollectionUtils.h"

#include <algorithm>

namespace PersonCollectionUtils {
    bool addPersonIfMissing(std::vector<Person>& people, const Person& person) {
        auto duplicate = std::find_if(people.begin(), people.end(), [&](const Person& existing) {
            return existing.getId() == person.getId();
        });

        if (duplicate != people.end()) {
            return false;
        }

        people.push_back(person);
        return true;
    }

    bool updatePersonById(std::vector<Person>& people, const std::string& id, const Person& updatedPerson) {
        auto target = std::find_if(people.begin(), people.end(), [&](const Person& person) {
            return person.getId() == id;
        });

        if (target == people.end()) {
            return false;
        }

        if (updatedPerson.getId() != id) {
            auto conflict = std::find_if(people.begin(), people.end(), [&](const Person& person) {
                return person.getId() == updatedPerson.getId();
            });

            if (conflict != people.end() && conflict != target) {
                return false;
            }
        }

        *target = updatedPerson;
        return true;
    }
}