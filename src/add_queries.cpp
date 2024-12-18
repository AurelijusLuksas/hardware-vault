#include <iostream>
#include "utility.h"

void addProduct() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_category_id;
    char c_name[100];
    char c_description[255];
    double c_price;
    int c_stock_quantity;

    EXEC SQL END DECLARE SECTION;


    std::string name = getString("Iveskite produkto pavadinima: ");
    std::string description = getString("Iveskite produkto aprasyma: ");
    double price = getDouble("Iveskite produkto kaina: ");
    int stock_quantity = getInt("Iveskite produkto kieki: ");


    std::cout << "Galimos kategorijos:\n";
    printCategory();
    int category_id = getInt("Iveskite produkto kategorijos ID: ");


    copyStr(name, c_name, sizeof(c_name));
    copyStr(description, c_description, sizeof(c_description));

    c_category_id = category_id;
    c_price = price;
    c_stock_quantity = stock_quantity;

    EXEC SQL INSERT INTO product (category_id, name, description, price, stock_quantity)
    VALUES (:c_category_id, :c_name, :c_description, :c_price, :c_stock_quantity);
    if (0 == SQLCODE) {
        EXEC SQL COMMIT;
        std::cout << "Produktas pridetas\n";
    } else {
        EXEC SQL ROLLBACK;
        std::cout << "Klaida pridedant produkta\n";
        std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
    }
}