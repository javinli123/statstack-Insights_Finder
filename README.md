# Insights Finder
The **insight generator algorithm** uses conditional probability to calculate the likelihood of trait B occurring given that trait A is already present. 

The rating represents the strength of the correlation. If there is a group of 50 people, and all 50 people have this correlation, the rating is 100%. If there is a group of 50 people, and only 40 people have this correlation, the rating is 80%. 

The app generates correlations for the following combinations:
* a primary OS of x having a study time of y
* a primary OS of x having a hobby of y
* a region of x having a study time of y
* a region of x having an engineering focus of y
* a region of x having a favorite color of y
* an engineering focus of x having a hobby of y
* an engineering focus of x having a favorite color of y

## Dataset Requirements

To be compatible with the Insight Generator algorithm, input data must conform to a predefined schema and use supported categorical values.

Several attributes (such as region, primary OS, engineering focus, and study time) are treated as **categorical variables** within the algorithm and are expected to map to known, finite sets of values. Data that does not conform to these expected categories may be rejected or ignored during parsing and analysis.

Other attributes (such as hobbies and favorite colors) are treated as free-form strings and are used directly for correlation generation.

Ensuring that input data adheres to these requirements allows the algorithm to produce meaningful and consistent correlation results.

## HTML Report Generation
Our Insight Generator also includes a feature that creates an HTML document presenting the findings in an aesthetically pleasing manner. 

## JsonPersonReader
JsonPersonReader makes an HTTP call to a configurable URL, retrieves JSON data, and parses it into a vector of Person objects.
```cpp
#include "JsonPersonReader.h"
JsonPersonReader reader(url);
```

### HTTP Request - using LibCurl
JsonPersonReader uses LibCurl with native Windows SSL support.
This allows secure HTTPS requests without requiring any manual certificate installation or extra configuration.

### JSON Parsing - using nlohmann/json.hpp

The reader parses JSON into typed Person objects using the popular single-header library: [nlohmann/json](https://github.com/nlohmann/json)

### Required JSON Structure
The JSON file must follow this structure:
```JSON
{
  "people": [
    {
      "id": string,
      "graduationYear": int,
      "region": string,
      "primaryOS": string,
      "engineeringFocus": string,
      "studyTime": string,
      "courseLoad": int,
      "favoriteColors": string,  // spinal-case or string
      "hobbies": string,
      "languages": string
    },

    ...
  ]
}
```
Each JSON object in the people array is converted into a Person instance.

### Error Handling
JsonPersonReader reports errors for:

- Failed CURL initialization
- Non-200 HTTP status codes
- Invalid JSON
- Missing "people" array

Failures are printed to `std::cerr`, and an empty vector is returned.

## Build and Run
This project uses **CMake** (3.10+) and **C++17**.

### Dependencies
- **LibCurl**: required to build and run the application (expected at `./libs/curl`)
- **Google Test (GTest)**: required to build and run unit tests (`find_package(GTest REQUIRED)`)

## Personal Copy
This repository is a personal working copy maintained by **Javin Li**.  
It is used for individual development, experimentation, and testing, and may differ from the official group submission.

### Contributors
**Team name:** statstack  
**Members:** Javin Li, Alyssa Lee, Camille Van Erp, Yanxi Yang

Boston University — ENG EC 327 (Fall 2025)