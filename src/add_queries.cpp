#include <iostream>
#include "utility.h"
#include <print>

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
        std::cout << "Klaida pridedant produkta\n";
        std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
        EXEC SQL ROLLBACK;
    }
}

void addCategory() {
    EXEC SQL BEGIN DECLARE SECTION;
    char c_name[100];
    char c_description[255];

    EXEC SQL END DECLARE SECTION;

    std::string name = getString("Iveskite kategorijos pavadinima: ");
    std::string description = getString("Iveskite kategorijos aprasyma: ");

    copyStr(name, c_name, sizeof(c_name));
    copyStr(description, c_description, sizeof(c_description));

    EXEC SQL INSERT INTO category (name, description)
    VALUES (:c_name, :c_description);
    if (0 == SQLCODE) {
        EXEC SQL COMMIT;
        std::cout << "Kategorija prideta\n";
    } else {
        std::cout << "Klaida pridedant kategorija\n";
        std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
        EXEC SQL ROLLBACK;
    }
}

void addCustomer() {
    EXEC SQL BEGIN DECLARE SECTION;
    char c_first_name[100];
    char c_last_name[100];
    char c_contact_info[255];
    EXEC SQL END DECLARE SECTION;

    std::string first_name = getString("Iveskite kliento varda: ");
    std::string last_name = getString("Iveskite kliento pavarde: ");
    std::string contact_info = getString("Iveskite kliento kontaktine informacija: ");

    copyStr(first_name, c_first_name, sizeof(c_first_name));
    copyStr(last_name, c_last_name, sizeof(c_last_name));
    copyStr(contact_info, c_contact_info, sizeof(c_contact_info));

    EXEC SQL INSERT INTO customer (first_name, last_name, contact_info)
    VALUES (:c_first_name, :c_last_name, :c_contact_info);
    if (0 == SQLCODE) {
        EXEC SQL COMMIT;
        std::cout << "Klientas pridetas\n";
    } else {
        std::cout << "Klaida pridedant klienta\n";
        std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
        EXEC SQL ROLLBACK;
    }

}
void addOrder() {
    EXEC SQL BEGIN DECLARE SECTION;
    char c_customer_id[100];
    int c_product_id;
    int c_quantity;
    EXEC SQL END DECLARE SECTION;

    std::string customer_id = getString("Iveskite kliento kontaktine informacija: ");
    printProduct();
    int product_id = getInt("Iveskite produkto ID: ");
    int quantity = getInt("Iveskite produkto kieki: ");

    copyStr(customer_id, c_customer_id, sizeof(c_customer_id));
    c_product_id = product_id;
    c_quantity = quantity;

    EXEC SQL INSERT INTO places_order (customer_id, product_id, count)
    VALUES (:c_customer_id, :c_product_id, :c_quantity);
    if (0 == SQLCODE) {
        EXEC SQL COMMIT;
        std::cout << "Uzsakymas pridetas\n";
    } else {
        std::cout << "Klaida pridedant uzsakyma\n";
        std::print("{}", sqlca.sqlerrm.sqlerrmc);
        std::cout << "\nKlaidos kodas: " << SQLCODE << std::endl;
        EXEC SQL ROLLBACK;
    }
}
void addWarehouse() {
    EXEC SQL BEGIN DECLARE SECTION;
    char c_address[100];
    int c_organization_id;
    EXEC SQL END DECLARE SECTION;

    std::string address = getString("Iveskite sandelio adresa: ");
    int organization_id = 1;

    copyStr(address, c_address, sizeof(c_address));
    c_organization_id = organization_id;

    EXEC SQL INSERT INTO warehouse (address, organization_id)
    VALUES (:c_address, :c_organization_id);
    if (0 == SQLCODE) {
        EXEC SQL COMMIT;
        std::cout << "Sandelis pridetas\n";
    } else {
        std::cout << "Klaida pridedant sandeli\n";
        std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
        EXEC SQL ROLLBACK;
    }
    

}
void addEmployee() {
    EXEC SQL BEGIN DECLARE SECTION;
    char c_position[255];
    int c_years_of_experience;
    char c_contact_info[255];
    char c_warehouse_id[255];
    char c_first_name[100];
    char c_last_name[100];
    EXEC SQL END DECLARE SECTION;

    std::string position = getString("Iveskite darbuotojo pareigas: ");
    int years_of_experience = getInt("Iveskite darbuotojo patirti: ");
    std::string contact_info = getString("Iveskite darbuotojo kontaktine informacija: ");
    printWarehouse();
    std::string warehouse_id = getString("Iveskite darbuotojo sandelio adresa: ");
    std::string first_name = getString("Iveskite darbuotojo varda: ");
    std::string last_name = getString("Iveskite darbuotojo pavarde: ");

    copyStr(position, c_position, sizeof(c_position));
    copyStr(contact_info, c_contact_info, sizeof(c_contact_info));
    copyStr(warehouse_id, c_warehouse_id, sizeof(c_warehouse_id));
    copyStr(first_name, c_first_name, sizeof(c_first_name));
    copyStr(last_name, c_last_name, sizeof(c_last_name));
    c_years_of_experience = years_of_experience;

    EXEC SQL INSERT INTO employee (position, years_of_experience, contact_info, warehouse_id, first_name, last_name)
    VALUES (:c_position, :c_years_of_experience, :c_contact_info, :c_warehouse_id, :c_first_name, :c_last_name);
    if (0 == SQLCODE) {
        EXEC SQL COMMIT;
        std::cout << "Darbuotojas pridetas\n";
    } else {
        std::cout << "Klaida pridedant darbuotoja\n";
        std::print("{}", sqlca.sqlerrm.sqlerrmc);
        std::cout << "\nKlaidos kodas: " << SQLCODE << std::endl;
        EXEC SQL ROLLBACK;
    }

}
void addSupplier() {
    EXEC SQL BEGIN DECLARE SECTION;
    char c_phone_number[100];
    char c_email[100];
    char c_name[100];
    float c_rating;
    char c_address[100];
    int c_product_id;
    double c_delivery_price;
    EXEC SQL END DECLARE SECTION;

    std::string phone_number = getString("Iveskite tiekejo telefono numeri: ");
    std::string email = getString("Iveskite tiekejo el. pasta: ");
    std::string name = getString("Iveskite tiekejo pavadinima: ");
    float rating = getFloat("Iveskite tiekejo ivertinima: ");
    std::string address = getString("Iveskite tiekejo adresa: ");
    printProduct();
    int product_id = getInt("Iveskite produkto ID: ");
    double delivery_price = getDouble("Iveskite pristatymo kaina: ");

    copyStr(phone_number, c_phone_number, sizeof(c_phone_number));
    copyStr(email, c_email, sizeof(c_email));
    copyStr(name, c_name, sizeof(c_name));
    copyStr(address, c_address, sizeof(c_address));
    c_rating = rating;
    c_product_id = product_id;
    c_delivery_price = delivery_price;

    EXEC SQL INSERT INTO supplier (phone_number, email, name, rating, address, product_id, delivery_price)
    VALUES (:c_phone_number, :c_email, :c_name, :c_rating, :c_address, :c_product_id, :c_delivery_price);
    if (0 == SQLCODE) {
        EXEC SQL COMMIT;
        std::cout << "Tiekejas pridetas\n";
    } else {
        std::cout << "Klaida pridedant tiekeja\n";
        std::print("{}", sqlca.sqlerrm.sqlerrmc);
        std::cout << "\nKlaidos kodas: " << SQLCODE << std::endl;
        EXEC SQL ROLLBACK;
    }
}

void addSpecification() {
    EXEC SQL BEGIN DECLARE SECTION;
    char c_name[100];
    char c_description[255];
    char c_value[255];
    int c_product_id;
    EXEC SQL END DECLARE SECTION;

    std::string name = getString("Iveskite specifikacijos pavadinima: ");
    std::string description = getString("Iveskite specifikacijos aprasyma: ");
    std::string value = getString("Iveskite specifikacijos reiksme: ");
    printProduct();
    int product_id = getInt("Iveskite produkto ID: ");

    copyStr(name, c_name, sizeof(c_name));
    copyStr(description, c_description, sizeof(c_description));
    copyStr(value, c_value, sizeof(c_value));
    c_product_id = product_id;

    EXEC SQL INSERT INTO specification (name, description, value, product_id)
    VALUES (:c_name, :c_description, :c_value, :c_product_id);
    if (0 == SQLCODE) {
        EXEC SQL COMMIT;
        std::cout << "Specifikacija prideta\n";
    } else {
        std::cout << "Klaida pridedant specifikacija\n";
        std::print("{}", sqlca.sqlerrm.sqlerrmc);
        std::cout << "\nKlaidos kodas: " << SQLCODE << std::endl;
        EXEC SQL ROLLBACK;
    }

}