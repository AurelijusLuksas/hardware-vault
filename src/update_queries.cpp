#include <iostream>
#include "utility.h"
#include <print>

void updateProductPrice() {
    EXEC SQL BEGIN DECLARE SECTION;
    int c_id;
    double c_price;
    EXEC SQL END DECLARE SECTION;


    printProduct();
    int product_id = getInt("Iveskite produkto ID, kurio kaina bus atnaujinta: ");
    c_id = product_id;

    double price = getDouble("Iveskite nauja produkto kaina: ");
    c_price = price;

    EXEC SQL UPDATE product SET price = :c_price WHERE id = :c_id;
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