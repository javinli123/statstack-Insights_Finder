#ifndef JSONPERSONREADER_H
#define JSONPERSONREADER_H

#include "PersonReader.h"
#include <string>
#include <vector>

class JsonPersonReader : public PersonReader {
private:
    std::string url_;
public:
    explicit JsonPersonReader(const std::string& url); // constructor
    std::vector<Person> read() override;
};

#endif // JSONPERSONREADER_H