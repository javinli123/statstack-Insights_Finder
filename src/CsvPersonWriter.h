#ifndef CSV_PERSON_WRITER_H
#define CSV_PERSON_WRITER_H

#include "Person.h"
#include <string>
#include <vector>

class CsvPersonWriter {
public:
    static bool write(const std::string& filename, const std::vector<Person>& people);
};

#endif