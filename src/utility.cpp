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

void copyStr(const std::string &str, char* c_str, size_t size) {
    auto copied = str.copy(c_str, size);
    c_str[copied] = '\0';
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
        printf("%d. %s - %s\n", category_id, category_name, category_description);
    }

    EXEC SQL CLOSE category_cursor;
}

void printProduct() {
    std::cout << "Galimi produktai:\n";

    EXEC SQL BEGIN DECLARE SECTION;
    int product_id;
    char product_name[100];
    char product_description[255];
    double product_price;
    int product_stock_quantity;
    EXEC SQL END DECLARE SECTION;

    EXEC SQL DECLARE product_cursor CURSOR FOR
        SELECT id, name, description, price, stock_quantity FROM product;

    EXEC SQL OPEN product_cursor;
    
    while(true) {
        EXEC SQL FETCH product_cursor INTO :product_id, :product_name, :product_description, :product_price, :product_stock_quantity;
        if (SQLCODE == 100) { // No more data
            break;
        } else if (SQLCODE != 0) { // Error
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant produktus\n";
            break;
        }
        printf("%d. %s - %s - %f - %d\n", product_id, product_name, product_description, product_price, product_stock_quantity);
    }

    EXEC SQL CLOSE product_cursor;
}

void printWarehouse() {
    std::cout << "Galimi sandeliai:\n";

    EXEC SQL BEGIN DECLARE SECTION;
    int warehouse_id;
    char address[100];
    int organization_id;
    EXEC SQL END DECLARE SECTION;

    EXEC SQL DECLARE warehouse_cursor CURSOR FOR
        SELECT id, address, organization_id FROM warehouse;

    EXEC SQL OPEN warehouse_cursor;
    
    while(true) {
        EXEC SQL FETCH warehouse_cursor INTO :warehouse_id, :address, :organization_id;
        if (SQLCODE == 100) { // No more data
            break;
        } else if (SQLCODE != 0) { // Error
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant sandelius\n";
            break;
        }
        printf("%d. %s - %d\n", warehouse_id, address, organization_id);
    }

    EXEC SQL CLOSE warehouse_cursor;
}

void printCustomer() {
    std::cout << "Galimi klientai:\n";

    EXEC SQL BEGIN DECLARE SECTION;
    int customer_id;
    int order_count;
    char contact_info[255];
    char first_name[100];
    char last_name[100];
    EXEC SQL END DECLARE SECTION;

    EXEC SQL DECLARE customer_cursor CURSOR FOR
        SELECT id, order_count, contact_info, first_name, last_name FROM customer;

    EXEC SQL OPEN customer_cursor;
    
    while(true) {
        EXEC SQL FETCH customer_cursor INTO :customer_id, :order_count, :contact_info, :first_name, :last_name;
        if (SQLCODE == 100) { // No more data
            break;
        } else if (SQLCODE != 0) { // Error
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant klientus\n";
            break;
        }
        printf("%d. %d - %s - %s - %s\n", customer_id, order_count, contact_info, first_name, last_name);
    }

    EXEC SQL CLOSE customer_cursor;
}

void printEmployee() {
    std::cout << "Galimi darbuotojai:\n";

    EXEC SQL BEGIN DECLARE SECTION;
    int employee_id;
    char position[100];
    int years_of_experience;
    char contact_info[255];
    char warehouse_id[100];
    char first_name[100];
    char last_name[100];
    EXEC SQL END DECLARE SECTION;

    EXEC SQL DECLARE employee_cursor CURSOR FOR
        SELECT id, position, years_of_experience, contact_info, warehouse_id, first_name, last_name FROM employee;

    EXEC SQL OPEN employee_cursor;

    while(true) {
        EXEC SQL FETCH employee_cursor INTO :employee_id, :position, :years_of_experience, :contact_info, :warehouse_id, :first_name, :last_name;
        if (SQLCODE == 100) { // No more data
            break;
        } else if (SQLCODE != 0) { // Error
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant darbuotojus\n";
            break;
        }
        printf("%d. %s - %d - %s - %s - %s - %s\n", employee_id, position, years_of_experience, contact_info, warehouse_id, first_name, last_name);
    }
}