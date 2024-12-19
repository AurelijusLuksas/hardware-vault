#include <iostream>
#include "utility.h"
#include <print>

void updateMaterializedProductStockSummary() {
    EXEC SQL BEGIN DECLARE SECTION;
    EXEC SQL END DECLARE SECTION;

    EXEC SQL REFRESH MATERIALIZED VIEW juja0400.product_stock_summary;
    if (SQLCODE != 0) {
        std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
        std::print("{}", sqlca.sqlerrm.sqlerrmc);
        std::cout << "Klaida atnaujinant produktų atsargų suvestinę\n";
        EXEC SQL ROLLBACK;
    } else {
        std::cout << "Atsargų suvestinė atnaujinta\n";
    }
}

void updateMaterializedProductRevenueLastWeek() {
    EXEC SQL BEGIN DECLARE SECTION;
    EXEC SQL END DECLARE SECTION;

    EXEC SQL REFRESH MATERIALIZED VIEW juja0400.all_product_revenue;
    if (SQLCODE != 0) {
        std::cout << "Klaidos kodas: " << SQLCODE << std::endl;
        std::print("{}", sqlca.sqlerrm.sqlerrmc);
        std::cout << "Klaida atnaujinant produktų pajamų suvestinę\n";
        EXEC SQL ROLLBACK;
    } else {
        std::cout << "Produktų pajamų suvestinė atnaujinta\n";
    }
}