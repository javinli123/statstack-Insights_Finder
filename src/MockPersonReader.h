#ifndef MOCKPERSONREADER_H
#define MOCKPERSONREADER_H

#include "PersonReader.h"
#include <vector>

class MockPersonReader : public PersonReader {
public:
    std::vector<Person> read() override;
};

#endif // MOCKPERSONREADER_H