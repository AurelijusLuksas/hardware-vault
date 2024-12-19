#include <iostream>
#include "utility.h"
#include <print>


void removeProduct() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_product_id;
    EXEC SQL END DECLARE SECTION;

    std::cout << "Galimi produktai:\n";
    printProduct();
    int product_id = getInt("Iveskite produkto ID: ");

    c_product_id = product_id;

    EXEC SQL DELETE FROM product WHERE id = :c_product_id;
    if (0 == SQLCODE) {
        EXEC SQL COMMIT;
        std::cout << "Produktas istrintas\n";
    } else {
        std::cout << "Klaida istrinant produkta\n";
        std::print("{}", sqlca.sqlerrm.sqlerrmc);
        std::cout << "\nKlaidos kodas: " << SQLCODE << std::endl;
        EXEC SQL ROLLBACK;
    }
}

void removeCategory() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_category_id;
    EXEC SQL END DECLARE SECTION;

    std::cout << "Galimos kategorijos:\n";
    printCategory();
    int category_id = getInt("Iveskite kategorijos ID: ");

    c_category_id = category_id;

    EXEC SQL DELETE FROM category WHERE id = :c_category_id;
    if (0 == SQLCODE) {
        EXEC SQL COMMIT;
        std::cout << "Kategorija istrinta\n";
    } else {
        std::cout << "Klaida istrinant kategorija\n";
        std::print("{}", sqlca.sqlerrm.sqlerrmc);
        std::cout << "\nKlaidos kodas: " << SQLCODE << std::endl;
        EXEC SQL ROLLBACK;
    }
}

void removeCustomer() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_customer_id;
    EXEC SQL END DECLARE SECTION;

    std::cout << "Galimi klientai:\n";
    printCustomer();
    int customer_id = getInt("Iveskite kliento ID: ");

    c_customer_id = customer_id;

    EXEC SQL DELETE FROM customer WHERE id = :c_customer_id;
    if (0 == SQLCODE) {
        EXEC SQL COMMIT;
        std::cout << "Klientas istrintas\n";
    } else {
        std::cout << "Klaida istrinant klienta\n";
        std::print("{}", sqlca.sqlerrm.sqlerrmc);
        std::cout << "\nKlaidos kodas: " << SQLCODE << std::endl;
        EXEC SQL ROLLBACK;
    }
}

void removeOrder() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_product_id;
    char c_customer_id[255];
    EXEC SQL END DECLARE SECTION;

    printCustomer();
    std::string customer_id = getString("Iveskite kliento kontaktine info kurio uzsakyma norite istrinti: ");
    int product_id = getInt("Iveskite produkto ID kurio uzsakyma norite istrinti: ");

    c_product_id = product_id;
    copyStr(customer_id, c_customer_id, sizeof(c_customer_id));

    EXEC SQL DELETE FROM places_order WHERE product_id = :c_product_id AND customer_id = :c_customer_id;
    EXEC SQL UPDATE customer SET order_count = order_count - 1 WHERE contact_info = :c_customer_id;
    if (0 == SQLCODE) {
        EXEC SQL COMMIT;
        std::cout << "Uzsakymas istrintas\n";
    } else {
        std::cout << "Klaida istrinant uzsakyma\n";
        std::print("{}", sqlca.sqlerrm.sqlerrmc);
        std::cout << "\nKlaidos kodas: " << SQLCODE << std::endl;
        EXEC SQL ROLLBACK;
    }
}

void removeWarehouse() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_warehouse_id;
    EXEC SQL END DECLARE SECTION;

    std::cout << "Galimi sandeliai:\n";
    printWarehouse();
    int warehouse_id = getInt("Iveskite sandelio ID: ");

    c_warehouse_id = warehouse_id;

    EXEC SQL DELETE FROM warehouse WHERE id = :c_warehouse_id;
    if (0 == SQLCODE) {
        EXEC SQL COMMIT;
        std::cout << "Sandelis istrintas\n";
    } else {
        std::cout << "Klaida istrinant sandeli\n";
        std::print("{}", sqlca.sqlerrm.sqlerrmc);
        std::cout << "\nKlaidos kodas: " << SQLCODE << std::endl;
        EXEC SQL ROLLBACK;
    }
}

void removeEmployee() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_employee_id;
    EXEC SQL END DECLARE SECTION;

    printEmployee();
    int employee_id = getInt("Iveskite darbuotojo ID: ");

    c_employee_id = employee_id;

    EXEC SQL DELETE FROM employee WHERE id = :c_employee_id;
    if (0 == SQLCODE) {
        EXEC SQL COMMIT;
        std::cout << "Darbuotojas istrintas\n";
    } else {
        std::cout << "Klaida istrinant darbuotoja\n";
        std::print("{}", sqlca.sqlerrm.sqlerrmc);
        std::cout << "\nKlaidos kodas: " << SQLCODE << std::endl;
        EXEC SQL ROLLBACK;
    }
}
