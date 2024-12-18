#include <iostream>
#include <string>
#include <limits>
#include <regex>
#include <limits>
#include <cstring>
#include <cstdlib>
#include "add_queries.h"

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

int getId(const std::string& prompt, const std::string& tableName) {
    EXEC SQL BEGIN DECLARE SECTION;
    char c_tableName[100];
    char query[200];
    int value;
    EXEC SQL END DECLARE SECTION;

    if (tableName.length() >= sizeof(c_tableName)) {
        std::cerr << "Table name too long.\n";
        exit(1);
    }

    strncpy(c_tableName, tableName.c_str(), sizeof(c_tableName) - 1);
    c_tableName[sizeof(c_tableName) - 1] = '\0';

    snprintf(query, sizeof(query), "SELECT * FROM %s", c_tableName);

    EXEC SQL PREPARE dyn_query FROM :query;

    // Execute the query
    EXEC SQL DECLARE cursor_name CURSOR FOR dyn_query;
    EXEC SQL OPEN cursor_name;

    std::cout << "Available IDs in " << tableName << ":\n";
    while (true) {
        EXEC SQL FETCH cursor_name INTO :value;
        if (SQLCODE != 0) break;
        std::cout << value << std::endl;
    }

    EXEC SQL CLOSE cursor_name;

    // Get user input for the ID
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Netinkamas ID. Bandykite dar karta.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear extra input
            return value;
        }
    }
}