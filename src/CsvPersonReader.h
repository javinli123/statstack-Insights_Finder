#ifndef CSVPERSONREADER_H
#define CSVPERSONREADER_H

#include "PersonReader.h"
#include <string>
#include <vector>
#include <unordered_set>

class CsvPersonReader : public PersonReader {
private:
    std::string filename_;
    static std::unordered_set<std::string> parseSet(const std::string& raw);
public:
    explicit CsvPersonReader(const std::string& filename);
    const std::string& getFilename() const { return filename_; }
    std::vector<Person> read() override;
};

#endif //CSVPERSONREADER_H