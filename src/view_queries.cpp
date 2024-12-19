#include <iostream>
#include "utility.h"
#include <print>

void viewLowStockProducts() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_id;
    char c_name[100];
    int c_stock_quantity;
    EXEC SQL END DECLARE SECTION;

    EXEC SQL DECLARE low_stock_cursor CURSOR FOR
    SELECT id, name, stock_quantity
    FROM juja0400.low_stock_products;
    EXEC SQL OPEN low_stock_cursor;

    while (true) {
        EXEC SQL FETCH low_stock_cursor INTO :c_id, :c_name, :c_stock_quantity;
        if (SQLCODE == 100) {
            break;
        }
        if (SQLCODE != 0) {
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant mazo atsargu kiekio produktus\n";
            break;
        } else {
            std::cout << "Produktas:\n";
            std::cout << "ID: " << c_id << std::endl;
            std::cout << "Pavadinimas: " << c_name << std::endl;
            std::cout << "Atsargu kiekis: " << c_stock_quantity << std::endl;
            std::cout << "-------------------\n";
        }
    }
    EXEC SQL CLOSE low_stock_cursor;
}

void viewSuppliersByRating() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_id;
    char c_name[100];
    float c_rating;
    char c_phone_number[100];
    char c_email[100];
    EXEC SQL END DECLARE SECTION;

    EXEC SQL DECLARE suppliers_rating_cursor CURSOR FOR
    SELECT id, name, rating, phone_number, email
    FROM juja0400.suppliers_by_rating;
    EXEC SQL OPEN suppliers_rating_cursor;

    while (true) {
        EXEC SQL FETCH suppliers_rating_cursor INTO :c_id, :c_name, :c_rating, :c_phone_number, :c_email;
        if (SQLCODE == 100) {
            break;
        }
        if (SQLCODE != 0) {
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant tiekejus pagal reitinga\n";
            break;
        } else {
            std::cout << "Tiekejas:\n";
            std::cout << "ID: " << c_id << std::endl;
            std::cout << "Pavadinimas: " << c_name << std::endl;
            std::cout << "Reitingas: " << c_rating << std::endl;
            std::cout << "Telefono numeris: " << c_phone_number << std::endl;
            std::cout << "El. pastas: " << c_email << std::endl;
            std::cout << "-------------------\n";
        }
    }
    EXEC SQL CLOSE suppliers_rating_cursor;
}

void viewActiveProducts() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_id;
    char c_name[100];
    double c_price;
    int c_stock_quantity;
    EXEC SQL END DECLARE SECTION;

    EXEC SQL DECLARE active_products_cursor CURSOR FOR
    SELECT id, name, price, stock_quantity
    FROM juja0400.active_products;
    EXEC SQL OPEN active_products_cursor;

    while (true) {
        EXEC SQL FETCH active_products_cursor INTO :c_id, :c_name, :c_price, :c_stock_quantity;
        if (SQLCODE == 100) {
            break;
        }
        if (SQLCODE != 0) {
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant aktyvius produktus\n";
            break;
        } else {
            std::cout << "Produktas:\n";
            std::cout << "ID: " << c_id << std::endl;
            std::cout << "Pavadinimas: " << c_name << std::endl;
            std::cout << "Kaina: " << c_price << std::endl;
            std::cout << "Atsargu kiekis: " << c_stock_quantity << std::endl;
            std::cout << "-------------------\n";
        }
    }
    EXEC SQL CLOSE active_products_cursor;
}

void viewHighRatedSuppliers() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_id;
    char c_name[100];
    float c_rating;
    EXEC SQL END DECLARE SECTION;
    
    EXEC SQL DECLARE high_rated_suppliers_cursor CURSOR FOR
    SELECT id, name, rating
    FROM juja0400.high_rated_suppliers;
    EXEC SQL OPEN high_rated_suppliers_cursor;

    while (true) {
        EXEC SQL FETCH high_rated_suppliers_cursor INTO :c_id, :c_name, :c_rating;
        if (SQLCODE == 100) {
            break;
        }
        if (SQLCODE != 0) {
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant aukstai ivertintus tiekejus\n";
            break;
        } else {
            std::cout << "Tiekejas:\n";
            std::cout << "ID: " << c_id << std::endl;
            std::cout << "Pavadinimas: " << c_name << std::endl;
            std::cout << "Reitingas: " << c_rating << std::endl;
            std::cout << "-------------------\n";
        }
    }
    EXEC SQL CLOSE high_rated_suppliers_cursor;
}

void viewLowStockProductsInWarehouse() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_warehouse_id;
    int c_product_id;
    char c_name[100];
    int c_stock_quantity;
    EXEC SQL END DECLARE SECTION;

    EXEC SQL DECLARE low_stock_warehouse_cursor CURSOR FOR
    SELECT warehouse_id, product_id, name, stock_quantity
    FROM juja0400.low_stock_products_in_warehouse;
    EXEC SQL OPEN low_stock_warehouse_cursor;

    while (true) {
        EXEC SQL FETCH low_stock_warehouse_cursor INTO :c_warehouse_id, :c_product_id, :c_name, :c_stock_quantity;
        if (SQLCODE == 100) {
            break;
        }
        if (SQLCODE != 0) {
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant mazo atsargu kiekio produktus sandelyje\n";
            break;
        } else {
            std::cout << "Produktas sandelyje:\n";
            std::cout << "Sandelio ID: " << c_warehouse_id << std::endl;
            std::cout << "Produkto ID: " << c_product_id << std::endl;
            std::cout << "Pavadinimas: " << c_name << std::endl;
            std::cout << "Atsargu kiekis: " << c_stock_quantity << std::endl;
            std::cout << "-------------------\n";
        }
    }
    EXEC SQL CLOSE low_stock_warehouse_cursor;
}

void viewLowestDeliveryPrice() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_id;
    char c_name[100];
    double c_delivery_price;
    char c_product_name[100];
    EXEC SQL END DECLARE SECTION;

    EXEC SQL DECLARE lowest_delivery_price_cursor CURSOR FOR
    SELECT id, name, delivery_price, product_name
    FROM juja0400.lowest_delivery_price;
    EXEC SQL OPEN lowest_delivery_price_cursor;

    while (true) {
        EXEC SQL FETCH lowest_delivery_price_cursor INTO :c_id, :c_name, :c_delivery_price, :c_product_name;
        if (SQLCODE == 100) {
            break;
        }
        if (SQLCODE != 0) {
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant maziausia pristatymo kaina\n";
            break;
        } else {
            std::cout << "Tiekejas:\n";
            std::cout << "ID: " << c_id << std::endl;
            std::cout << "Pavadinimas: " << c_name << std::endl;
            std::cout << "Pristatymo kaina: " << c_delivery_price << std::endl;
            std::cout << "Produktas kuri pristato tiekejas: " << c_product_name << std::endl;
            std::cout << "-------------------\n";
        }
    }
    EXEC SQL CLOSE lowest_delivery_price_cursor;
}

void viewProductStockSummary() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_product_id;
    char c_product_name[100];
    int c_stock_quantity;
    double c_stock_value;
    EXEC SQL END DECLARE SECTION;

    EXEC SQL DECLARE product_stock_summary_cursor CURSOR FOR
    SELECT product_id, product_name, stock_quantity, stock_value
    FROM juja0400.product_stock_summary;
    EXEC SQL OPEN product_stock_summary_cursor;

    while (true) {
        EXEC SQL FETCH product_stock_summary_cursor INTO :c_product_id, :c_product_name, :c_stock_quantity, :c_stock_value;
        if (SQLCODE == 100) {
            break;
        }
        if (SQLCODE != 0) {
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant produktu atsargu suvestine\n";
            break;
        } else {
            std::cout << "Produkto atsargu suvestine:\n";
            std::cout << "Produkto ID: " << c_product_id << std::endl;
            std::cout << "Produkto pavadinimas: " << c_product_name << std::endl;
            std::cout << "Atsargu kiekis: " << c_stock_quantity << std::endl;
            std::cout << "Atsargu verte: " << c_stock_value << std::endl;
            std::cout << "-------------------\n";
        }
    }
    EXEC SQL CLOSE product_stock_summary_cursor;
}

void viewProductRevenueLastWeek() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_product_id;
    char c_product_name[100];
    double c_total_revenue;
    EXEC SQL END DECLARE SECTION;

    EXEC SQL DECLARE product_revenue_last_week_cursor CURSOR FOR
    SELECT product_id, product_name, total_revenue
    FROM juja0400.all_product_revenue;
    EXEC SQL OPEN product_revenue_last_week_cursor;

    while (true) {
        EXEC SQL FETCH product_revenue_last_week_cursor INTO :c_product_id, :c_product_name, :c_total_revenue;
        if (SQLCODE == 100) {
            break;
        }
        if (SQLCODE != 0) {
            std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
            std::print("{}", sqlca.sqlerrm.sqlerrmc);
            std::cout << "Klaida gaunant produktu pajamas per praejusia savaite\n";
            break;
        } else {
            std::cout << "Produkto pajamos per praejusia savaite:\n";
            std::cout << "Produkto ID: " << c_product_id << std::endl;
            std::cout << "Produkto pavadinimas: " << c_product_name << std::endl;
            std::cout << "Bendros pajamos: " << c_total_revenue << std::endl;
            std::cout << "-------------------\n";
        }
    }
    EXEC SQL CLOSE product_revenue_last_week_cursor;
}