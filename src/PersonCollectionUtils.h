#ifndef PERSON_COLLECTION_UTILS_H
#define PERSON_COLLECTION_UTILS_H

#include "Person.h"
#include <string>
#include <vector>

namespace PersonCollectionUtils {
    bool addPersonIfMissing(std::vector<Person>& people, const Person& person);

    bool updatePersonById(std::vector<Person>& people, const std::string& id, const Person& updatedPerson);
}

#endif