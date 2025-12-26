#include "JsonPersonReader.h"
#include "Person.h"
#include "PersonEnums.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <ostream>
#include <sstream>
#include <unordered_set>
#include <curl/curl.h>

using json = nlohmann::json;
using namespace std;

JsonPersonReader::JsonPersonReader(const string& url) {
    url_ = url;
}

/*
 * A helper function that helps to separate a spinal case phrase into a list containing each elements of the spinal phrase.
 * For example, "blue-red-greed" -> {"blue", "red", "green"}
 */
static unordered_set<std::string> splitTags(const string& input, char sep = '-') {
    unordered_set<string> result;
    stringstream ss(input);
    string item;

    // Split the string at each separator
    while (getline(ss, item, sep)) {
        if (!item.empty()) result.insert(item); // ignore empty items
    }

    return result;
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::vector<Person> JsonPersonReader::read() {
    std::vector<Person> people;
    std::cout << "JsonPersonReader: Reading from " << url_ << std::endl;
    /**
    * Make HTTP Request
    * using third party: libcurl
    */
    // ---- CURL HTTP REQUEST ----
    CURL* curl = curl_easy_init();
    string response;

    if (!curl) {
        cerr << "CURL initialization failed." << endl;
        return people;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Ignore SSL verification
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    // HTTP Request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "CURL error: " << curl_easy_strerror(res) << endl;
        curl_easy_cleanup(curl);
        return people;
    }

    curl_easy_cleanup(curl);

    // Parse JSON
    json root;
    try {
        root = json::parse(response);
    } catch (std::exception& e) {
        cerr << "JSON parse error: " << e.what() << endl;
        return people;
    }

    cout << "HTTP Request complete" << endl;

    // Allow either:
    // 1. array ( [ { person }, ... ] )
    // 2. object with "people": [ { person }, ... ]
    json list;
    if (root.is_array()) {
        list = root;
    }
    else if (root.contains("people") && root["people"].is_array()) {
        list = root["people"];
    }
    else {
        cerr << "JSON format error: expected array or {\"people\": [...]}" << endl;
        return people;
    }

    // Build Person objects
    for (auto& p : list) {
        string id = p["id"].get<string>();
        int graduationYear = p["graduationYear"].get<int>();

        Region region = parse_region(p["region"].get<string>());
        PrimaryOS os = parse_primary_os(p["primaryOS"].get<string>());
        EngineeringFocus focus = parse_engineering_focus(p["engineeringFocus"].get<string>());
        StudyTime time = parse_study_time(p["studyTime"].get<string>());

        int courseLoad = p["courseLoad"].get<int>();

        unordered_set<string> favoriteColors, hobbies, languages;

        // favoriteColors
        if (p.contains("favoriteColors")) {
            if (p["favoriteColors"].is_array()) {
                for (auto& c : p["favoriteColors"]) favoriteColors.insert(c.get<string>());
            } else {
                favoriteColors = splitTags(p["favoriteColors"].get<string>());
            }
        }

        // hobbies
        if (p.contains("hobbies")) {
            if (p["hobbies"].is_array()) {
                for (auto& h : p["hobbies"]) hobbies.insert(h.get<string>());
            } else {
                hobbies = splitTags(p["hobbies"].get<string>());
            }
        }

        // languages
        if (p.contains("languages")) {
            if (p["languages"].is_array()) {
                for (auto& l : p["languages"]) languages.insert(l.get<string>());
            } else {
                languages = splitTags(p["languages"].get<string>());
            }
        }

        // Construct a Person
        people.emplace_back(
            id, graduationYear, region, os, focus, time, courseLoad,
            favoriteColors, hobbies, languages
        );
    }

    return people;
}