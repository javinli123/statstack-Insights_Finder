#ifndef HTMLREPORTWRITER_H
#define HTMLREPORTWRITER_H

#include "Insight.h"
#include <vector>
#include <string>

// Static service to generate a pretty HTML dashboard
class HtmlReportWriter {
public:
    static void writeReport(const std::string& filename, const std::vector<Insight>& insights);
};

#endif // HTMLREPORTWRITER_H