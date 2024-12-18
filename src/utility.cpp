#include <iostream>
#include <string>
#include <limits>
#include <regex>
#include <limits>
#include <cstring>
#include <cstdlib>
#include "add_queries.h"
#include <print>

// Utility function to get an integer input
int getInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Netinkamas skaicius. Bandykite dar karta.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear extra input
            return value;
        }
    }
}

// Utility function to get a float input
float getFloat(const std::string& prompt) {
    float value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Netinkamas skaicius. Bandykite dar karta.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear extra input
            return value;
        }
    }
}

// Utility function to get a double input
double getDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Netinkamas skaicius. Bandykite dar karta.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear extra input
            return value;
        }
    }
}

// Utility function to get a string input
std::string getString(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);

    // Ensure input is not empty
    while (value.empty()) {
        std::cout << "Ivestis negali buti tuscia. Bandykite dar karta.\n";
        std::cout << prompt;
        std::getline(std::cin, value);
    }
    return value;
}

// Utility function to get a date input (YYYY-MM-DD format)
std::string getDate(const std::string& prompt) {
    std::string value;
    std::regex datePattern(R"(^\d{4}-\d{2}-\d{2}$)"); // Regex for YYYY-MM-DD format

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);

        if (std::regex_match(value, datePattern)) {
            return value;
        } else {
            std::cout << "Netinkama data. Naudokite formata YYYY-MM-DD. Bandykite dar karta.\n";
        }
    }
}

void printCategory() {
    std::cout << "Galimos kategorijos:\n";

    EXEC SQL BEGIN DECLARE SECTION;
    int category_id;
    char category_name[100];
    char category_description[255];
    EXEC SQL END DECLARE SECTION;

    EXEC SQL DECLARE category_cursor CURSOR FOR
        SELECT id, name, description FROM category;

    EXEC SQL OPEN category_cursor;
    
    while(true) {
        EXEC SQL FETCH category_cursor INTO :category_id, :category_name, :category_description;
        if (SQLCODE == 100) { // No more data
            break;
        } else if (SQLCODE != 0) { // Error
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant kategorijas\n";
            break;
        }
        std::cout << "AAAAAA\n";
        printf("%d. %s - %s\n", category_id, category_name, category_description);
    }


}