#include <iostream>
#include <memory>
#include <string>
#include <ctime>
#include <cstdlib>

#include "src/InsightsApp.h"
#include "src/MockPersonReader.h"
#include "src/RandomPersonReader.h"
#include "src/CsvPersonReader.h"
#include "src/JsonPersonReader.h"

void printMenu() {
    std::cout << "\n=== Insights Finder ===" << std::endl;
    std::cout << "Select Data Source:" << std::endl;
    std::cout << "1. Mock Data" << std::endl;
    std::cout << "2. Random Data" << std::endl;
    std::cout << "3. CSV File (class_data_set.csv)" << std::endl;
    std::cout << "4. JSON File (Live from GitHub)" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "> ";
}

int main() {
    std::unique_ptr<PersonReader> selectedReader = nullptr; // polymorphism

    std::srand(static_cast<unsigned>(std::time(nullptr))); // needed to guarantee complete randomness

    printMenu();
    int choice;
    std::cin >> choice;

    if (choice == 0) {
        std::cout << "Exiting..." << std::endl;

    } else if (choice == 1) {
        std::cout << "Loading Mock Data..." << std::endl;
        selectedReader = std::make_unique<MockPersonReader>();

    } else if (choice == 2) {
        std::cout << "How many random people should be generated? > ";
        int count;
        std::cin >> count;

        if (count <= 0) {
            std::cout << "Invalid count. Using default of 10.\n";
            count = 10;
        }

        std::cout << "Loading Random Data..." << std::endl;
        selectedReader = std::make_unique<RandomPersonReader>(count);

    } else if (choice == 3) {
        std::string filename;
        std::cout << "Enter CSV filename (default: class_data_set.csv) > ";
        std::cin >> filename;

        // Just hit enter for default
        if (filename.empty()) {
            filename = "class_data_set.csv";
        }
        std::cout << "Loading CSV: " << filename << std::endl;
        selectedReader = std::make_unique<CsvPersonReader>(filename);

    } else if (choice == 4) {
        std::string url = "http://gist.github.com/esolovey-bu/cba6c1b4eedd0a621ce879e6e6299d28/raw/sample_people.json";
        std::cout << "Loading URL: " << url << std::endl;
        selectedReader = std::make_unique<JsonPersonReader>(url);

    } else {
        std::cout << "Invalid choice. Terminating..." << std::endl;
    }

    // DEPENDENCY INJECTION: pass the chosen reader into the App
    if (selectedReader) {
        InsightsApp app(std::move(selectedReader));
        app.run();
    }
    return 0;
}