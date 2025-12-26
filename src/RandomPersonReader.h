#ifndef RANDOMPERSONREADER_H
#define RANDOMPERSONREADER_H

#include "PersonReader.h"

class RandomPersonReader : public PersonReader {
public:
    explicit RandomPersonReader(int count);

    std::vector<Person> read() override;

private:
    int n;
    Person randomPerson();
};

#endif //RANDOMPERSONREADER_H