#include <iostream>
#include "utility.h"
#include <print>

void searchEmployee() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_id;
    char c_position[100];
    int c_years_of_experience;
    char c_contact_info[100];
    char c_warehouse[100];
    char c_first_name[100];
    char c_last_name[100];
    EXEC SQL END DECLARE SECTION;

    std::string contact_info = getString("Iveskite darbuotojo kontaktine informacija: ");
    copyStr(contact_info, c_contact_info, sizeof(c_contact_info));

    EXEC SQL DECLARE employee_cursor CURSOR FOR
    SELECT id, position, years_of_experience, contact_info, warehouse_id, first_name, last_name FROM employee WHERE contact_info = :c_contact_info;
    EXEC SQL OPEN employee_cursor;

    while (true) {
        EXEC SQL FETCH employee_cursor INTO :c_id, :c_position, :c_years_of_experience, :c_contact_info, :c_warehouse, :c_first_name, :c_last_name;
        if (SQLCODE == 100) {
            break;
        }
        if (SQLCODE != 0) {
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant darbuotoja\n";
            EXEC SQL ROLLBACK;
            break;
        } else {
            std::cout << "Darbuotojas:\n";
            std::cout << "ID: " << c_id << std::endl;
            std::cout << "Pozicija: " << c_position << std::endl;
            std::cout << "Metai patirties: " << c_years_of_experience << std::endl;
            std::cout << "Kontaktine informacija: " << c_contact_info << std::endl;
            std::cout << "Darbo vieta: " << c_warehouse << std::endl;
            std::cout << "Vardas: " << c_first_name << std::endl;
            std::cout << "Pavarde: " << c_last_name << std::endl;
        }
    }
    EXEC SQL CLOSE employee_cursor;
}

void searchProductSpecification(int id) {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_id;
    char c_name[100];
    char c_description[255];
    char c_value[255];
    int c_product_id;
    EXEC SQL END DECLARE SECTION;

    c_product_id = id;

    EXEC SQL DECLARE product_specification_cursor CURSOR FOR
    SELECT id, name, description, value FROM specification WHERE product_id = :c_product_id;
    EXEC SQL OPEN product_specification_cursor;

    while (true) {
        EXEC SQL FETCH product_specification_cursor INTO :c_id, :c_name, :c_description, :c_value;
        if (SQLCODE == 100) {
            break;
        }
        if (SQLCODE != 0) {
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant produkto specifikacija\n";
            EXEC SQL ROLLBACK;
            break;
        } else {
            std::cout << "Specifikacija: " << c_name << " " << c_description << " " << c_value << std::endl;
        }
    }
    EXEC SQL CLOSE product_specification_cursor;
}

std::string getCategory(int id) {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_id;
    char c_name[100];
    char c_description[255];
    EXEC SQL END DECLARE SECTION;

    c_id = id;

    EXEC SQL DECLARE category_cursor CURSOR FOR
    SELECT id, name, description FROM category WHERE id = :c_id;
    EXEC SQL OPEN category_cursor;

    while (true) {
        EXEC SQL FETCH category_cursor INTO :c_id, :c_name, :c_description;
        if (SQLCODE == 100) {
            break;
        }
        if (SQLCODE != 0) {
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant kategorija\n";
            EXEC SQL ROLLBACK;
            break;
        } else {
            return c_name;
        }
    }
    EXEC SQL CLOSE category_cursor;
    return "";
}

void searchProduct() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_id;
    int c_category_id;
    char c_name[100];
    char c_description[255];
    double c_price;
    int c_stock_quantity;
    EXEC SQL END DECLARE SECTION;

    std::string name = getString("Iveskite produkto pavadinima: ");
    copyStr(name, c_name, sizeof(c_name));

    EXEC SQL DECLARE product_cursor CURSOR FOR
    SELECT id, category_id, name, description, price, stock_quantity FROM product WHERE name = :c_name;
    EXEC SQL OPEN product_cursor;

    while (true) {
        EXEC SQL FETCH product_cursor INTO :c_id, :c_category_id, :c_name, :c_description, :c_price, :c_stock_quantity;
        if (SQLCODE == 100) {
            break;
        }
        if (SQLCODE != 0) {
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant produkta\n";
            EXEC SQL ROLLBACK;
            break;
        } else {
            std::cout << "Produktas:\n";
            std::cout << "ID: " << c_id << std::endl;
            std::cout << "Kategorija: " << getCategory(c_id) << std::endl;
            std::cout << "Pavadinimas: " << c_name << std::endl;
            std::cout << "Aprasymas: " << c_description << std::endl;
            std::cout << "Kaina: " << c_price << std::endl;
            std::cout << "Kiekis: " << c_stock_quantity << std::endl;
            searchProductSpecification(c_id);
            break;
        }
    }
    EXEC SQL CLOSE product_cursor;
}

void searchOrder() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_id;
    char c_customer_id[100];
    int c_product_id;
    float c_price;
    char c_date[100];
    int c_quantity;
    EXEC SQL END DECLARE SECTION;

    std::string date = getDate("Iveskite uzsakymo data: ");
    copyStr(date, c_date, sizeof(c_date));

    EXEC SQL DECLARE order_cursor CURSOR FOR
    SELECT id, customer_id, product_id, price, date, count FROM places_order WHERE date = :c_date;
    EXEC SQL OPEN order_cursor;

    while (true) {
        EXEC SQL FETCH order_cursor INTO :c_id, :c_customer_id, :c_product_id, :c_price, :c_date, :c_quantity;
        if (SQLCODE == 100) {
            break;
        }
        if (SQLCODE != 0) {
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant uzsakyma\n";
            EXEC SQL ROLLBACK;
            break;
        } else {
            std::cout << "Uzsakymas:\n";
            std::cout << "ID: " << c_id << std::endl;
            std::cout << "Kliento ID: " << c_customer_id << std::endl;
            std::cout << "Produkto ID: " << c_product_id << std::endl;
            std::cout << "Uzsakymo kaina: " << c_price << std::endl;
            std::cout << "Uzsakymo data: " << c_date << std::endl;
            std::cout << "Uzsakyto produkto kiekis: " << c_quantity << std::endl;
            break;
        }
    }
    EXEC SQL CLOSE order_cursor;
}

void searchProductSpecification() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_id;
    char c_name[100];
    char c_description[255];
    char c_value[255];
    int c_product_id;
    EXEC SQL END DECLARE SECTION;

    int product_id = getInt("Iveskite produkto ID, kurio specifikacija norima pamatyti: ");
    c_product_id = product_id;

    EXEC SQL DECLARE specification_cursor CURSOR FOR
    SELECT id, name, description, value, product_id FROM specification WHERE product_id = :c_product_id;
    EXEC SQL OPEN specification_cursor;

    while (true) {
        EXEC SQL FETCH specification_cursor INTO :c_id, :c_name, :c_description, :c_value, :c_product_id;
        if (SQLCODE == 100) {
            break;
        }
        if (SQLCODE != 0) {
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant produkto specifikacija\n";
            EXEC SQL ROLLBACK;
            break;
        } else {
            std::cout << "Specifikacija: " << c_name << " " << c_description << " " << c_value << std::endl;
            break;
        }
    }
    EXEC SQL CLOSE specification_cursor;
}

void searchProductsOfCategory() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_id;
    int c_category_id;
    char c_name[100];
    char c_description[255];
    double c_price;
    int c_stock_quantity;
    EXEC SQL END DECLARE SECTION;

    printCategory();


    int category_id = getInt("Iveskite kategorijos ID, kurios produktus norite pamatyti: ");
    c_category_id = category_id;

    EXEC SQL DECLARE product_category CURSOR FOR

    SELECT id, category_id, name, description, price, stock_quantity FROM product WHERE category_id = :c_category_id;
    EXEC SQL OPEN product_category;
    
    while (true) {
        EXEC SQL FETCH product_category INTO :c_id, :c_category_id, :c_name, :c_description, :c_price, :c_stock_quantity;
        if (SQLCODE == 100) {
            break;
        }
        if (SQLCODE != 0) {
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant produkta\n";
            EXEC SQL ROLLBACK;
            break;
        } else {
            std::cout << "Produktas:\n";
            std::cout << "ID: " << c_id << std::endl;
            std::cout << "Pavadinimas: " << c_name << std::endl;
        }
    }
    EXEC SQL CLOSE product_category;
}
