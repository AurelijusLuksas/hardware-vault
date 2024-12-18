#include <iostream>

void copyStr(const std::string &str, char* c_str, size_t size) {
    auto copied = str.copy(c_str, size);
    c_str[copied] = '\0';
}

void addProduct(int category_id, const std::string& name, const std::string& description, double price, int stock_quantity) {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_category_id;
    char c_name[100];
    char c_description[255];
    double c_price;
    int c_stock_quantity;
    EXEC SQL END DECLARE SECTION;

    copyStr(name, c_name, sizeof(c_name));
    copyStr(description, c_description, sizeof(c_description));

    c_category_id = category_id;
    c_price = price;
    c_stock_quantity = stock_quantity;

    EXEC SQL INSERT INTO product (category_id, name, description, price, stock_quantity)
    VALUES (:c_category_id, :c_name, :c_description, :c_price, :c_stock_quantity);
    if (0 == SQLCODE) {
        std::cout << "Produktas pridetas\n";
    } else {
        std::cout << "Klaida pridedant produkta\n";
        std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
    }

}