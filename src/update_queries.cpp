#include <iostream>
#include "utility.h"
#include <print>
#include "search_queries.h"

void updateProductPrice() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_product_id;
    double c_price;
    EXEC SQL END DECLARE SECTION;


    printProduct();
    int product_id = getInt("Iveskite produkto ID, kurio kaina bus atnaujinta: ");
    c_product_id = product_id;

    double price = getDouble("Iveskite nauja produkto kaina: ");
    c_price = price;

    EXEC SQL UPDATE product SET price = :c_price WHERE id = :c_product_id;
    if (SQLCODE != 0) {
        std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
        std::print("{}", sqlca.sqlerrm.sqlerrmc);
        std::cout << "Klaida atnaujinant produkto kaina\n";
        EXEC SQL ROLLBACK;
    } else {
        std::cout << "Produkto kaina atnaujinta\n";
        EXEC SQL COMMIT;
    }
}

void updateProductQuantity() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_product_id;
    int c_stock_quantity;
    int old_stock_quantity;
    int difference;
    int warehouse_id;
    EXEC SQL END DECLARE SECTION;
    

    printProduct();
    int product_id = getInt("Iveskite produkto ID, kurio kiekis bus atnaujintas: ");
    c_product_id = product_id;
    int stock_quantity = getInt("Iveskite nauja produkto kieki: ");
    c_stock_quantity = stock_quantity;

    // Update the product's stock quantity
    EXEC SQL SELECT stock_quantity INTO :old_stock_quantity FROM product WHERE id = :c_product_id;
    EXEC SQL UPDATE product SET stock_quantity = :c_stock_quantity WHERE id = :c_product_id;
    difference = c_stock_quantity - old_stock_quantity;
    std::cout << "Difference: " << difference << std::endl;
    // Declare and open the cursor for warehouses
    EXEC SQL DECLARE warehouse_cursor CURSOR FOR 
        SELECT warehouse_id, stock_quantity FROM has_product WHERE product_id = :c_product_id ORDER BY stock_quantity ASC;
    EXEC SQL OPEN warehouse_cursor;

    EXEC SQL FETCH warehouse_cursor INTO :warehouse_id;
     // No more rows

        if (difference > 0) {
            std::cout << "Difference loope: " << difference << std::endl;
            // Add stock to this warehouse
            EXEC SQL UPDATE has_product SET stock_quantity = stock_quantity + :difference 
                WHERE product_id = :c_product_id AND warehouse_id = :warehouse_id;
            difference = 0; // Stock fulfilled
        } 

    EXEC SQL CLOSE warehouse_cursor;
    
    EXEC SQL DECLARE warehouse_cursor4 CURSOR FOR 
        SELECT warehouse_id, stock_quantity FROM has_product WHERE product_id = :c_product_id ORDER BY stock_quantity DESC;
    EXEC SQL OPEN warehouse_cursor4;

    EXEC SQL FETCH warehouse_cursor4 INTO :warehouse_id;
        if (difference < 0) {
            // Remove stock from this warehouse
            EXEC SQL UPDATE has_product SET stock_quantity = stock_quantity + :difference 
                WHERE product_id = :c_product_id AND warehouse_id = :warehouse_id;
            difference = 0;
        }

    EXEC SQL CLOSE warehouse_cursor4;
    if (SQLCODE != 0) {
        std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
        std::print("{}", sqlca.sqlerrm.sqlerrmc);
        std::cout << "Klaida atnaujinant produkto kieki\n";
        EXEC SQL ROLLBACK;
    } else {
        std::cout << "Produkto kiekis atnaujintas\n";
        EXEC SQL COMMIT;
    }
}

void updateProductCountInOrder() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_order_id;
    int c_product_id;
    int c_stock_quantity;
    int c_old_stock_quantity;
    int difference;
    int c_warehouse_id;
    EXEC SQL END DECLARE SECTION;

    printCustomer();
    searchCustomerOrders();

    int order_id = getInt("Iveskite uzsakymo ID, kurio produkto kiekis bus atnaujintas: ");
    c_order_id = order_id;

    int stock_quantity = getInt("Iveskite nauja produkto kieki: ");
    c_stock_quantity = stock_quantity;

    // Update the order's product count
    EXEC SQL SELECT product_id, count INTO :c_product_id, :c_old_stock_quantity FROM places_order WHERE id = :c_order_id;
    difference = c_stock_quantity - c_old_stock_quantity;

    EXEC SQL UPDATE places_order SET count = :c_stock_quantity WHERE id = :c_order_id;

    // Update the product's stock quantity
    EXEC SQL UPDATE product SET stock_quantity = :c_stock_quantity - :difference WHERE id = :c_product_id;
    
    // Update the product's stock quantity in has_product

    std::cout <<"produkt id " << c_product_id << std::endl;

    EXEC SQL DECLARE warehouse_cursor3 CURSOR FOR 
        SELECT warehouse_id, stock_quantity FROM has_product WHERE product_id = :c_product_id ORDER BY stock_quantity ASC;
    EXEC SQL OPEN warehouse_cursor3;
    
    std:: cout << "warehouse idaaaaaa" << std::endl;
    EXEC SQL FETCH warehouse_cursor3 INTO :c_warehouse_id;
     // No more rows
        std:: cout << "warehouse id: " << c_warehouse_id << std::endl;
        if (difference < 0) {
                        std::cout << "<0Difference loope: " << difference << std::endl;
            
            // Add stock to this warehouse
            EXEC SQL UPDATE has_product SET stock_quantity = stock_quantity + :difference 
                WHERE product_id = :c_product_id AND warehouse_id = :c_warehouse_id;
            difference = 0; // Stock fulfilled
        } 

    EXEC SQL CLOSE warehouse_cursor3;
    
    EXEC SQL DECLARE warehouse_cursor5 CURSOR FOR 
        SELECT warehouse_id, stock_quantity FROM has_product WHERE product_id = :c_product_id ORDER BY stock_quantity DESC;
    EXEC SQL OPEN warehouse_cursor5;

    EXEC SQL FETCH warehouse_cursor5 INTO :c_warehouse_id;
        if (difference > 0) {
            std::cout << "Difference loope: " << difference << std::endl;
            // Remove stock from this warehouse
            EXEC SQL UPDATE has_product SET stock_quantity = stock_quantity + :difference 
                WHERE product_id = :c_product_id AND warehouse_id = :c_warehouse_id;
            difference = 0;
        }

    EXEC SQL CLOSE warehouse_cursor5;

    if (SQLCODE != 0) {
        std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
        std::print("{}", sqlca.sqlerrm.sqlerrmc);
        std::cout << "Klaida atnaujinant produkto kieki uzsakyme\n";
        EXEC SQL ROLLBACK;
    } else {
        std::cout << "Produkto kiekis uzsakyme atnaujintas\n";
        EXEC SQL COMMIT;
    }

}