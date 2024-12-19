#include <iostream>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <fstream>
#include <sstream>
#include "add_queries.h"
#include "search_queries.h"
#include "remove_queries.h"
#include "update_queries.h"

#define MAX_PASSWORD_LENGTH 100

EXEC SQL BEGIN DECLARE SECTION;
int result;
char password[MAX_PASSWORD_LENGTH];
char username[9] = "";
EXEC SQL END DECLARE SECTION;

int main() {
    printf("Programa pradeda darba\n");

    std::ifstream envFile(".env");
    std::string line;
    while (std::getline(envFile, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            if (key == "password") {
                strncpy(password, value.c_str(), sizeof(password) - 1);
                password[sizeof(password) - 1] = '\0';
            } else if (key == "username") {
                strncpy(username, value.c_str(), sizeof(username) - 1);
                username[sizeof(username) - 1] = '\0';
            }
        }
    }

    EXEC SQL CONNECT TO studentu@localhost:5432 USER :username USING :password;

    printf("CONNECT TO %s. SQLCODE=%ld\n", username, SQLCODE);
    if (0 == SQLCODE) {
        printf("Prisijungta prie duomenu bazes. Galite pradeti darba.\n");
    } else {
        printf("SQLCODE=%ld\n", SQLCODE);
    }


    while (true) {
        std::cout << "[1] Duomenu paieska\n";
        std::cout << "[2] Duomenu ivedimas\n";
        std::cout << "[3] Duomenu atnaujinimas\n";
        std::cout << "[4] Duomenu trynimas\n";
        std::cout << "[0] Isjungti\n";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Duomenu paieska\n";
                while (true) {
                    std::cout << "-------------------\n";
                    std::cout << "[1] Rasti darbuotoja\n";
                    std::cout << "[2] Rasti produkta\n";
                    std::cout << "[3] Rasti uzsakyma\n";
                    std::cout << "[4] Rasti produkto specifikacija\n";
                    std::cout << "[5] Rasti kategorijos produktus\n";
                    std::cout << "[6] Rasti produkto tiekejus\n";
                    std::cout << "[7] Rasti produkto kieki sandelyje\n";
                    std::cout << "[8] Rasti kliento uzsakymus\n";
                    std::cout << "[0] Grizti\n";

                    int choiceFind;
                    std::cin >> choiceFind;

                    switch (choiceFind) {
                        case 1:
                            searchEmployee();
                            break;
                        case 2:
                            searchProduct();
                            break;
                        case 3:
                            searchOrder();
                            break;
                        case 4:
                            searchProductSpecification();
                            break;
                        case 5:
                            searchProductsOfCategory();
                            break;
                        case 6:
                            searchProductSupplier();
                            break;
                        case 7:
                            searchProductWarehouse();
                            break;
                        case 8:
                            searchCustomerOrders();
                            break;
                        case 0:
                            std::cout << "Grizti\n";
                            break;
                        default:
                            std::cout << "Neteisingas pasirinkimas\n";
                            break;
                    }
                    if (choiceFind == 0) {
                        break;
                    }
                }

                break;
            case 2:
                std::cout << "Duomenu ivedimas\n";
                while (true) {
                    std::cout << "-------------------\n";
                    std::cout << "[1] Prideti nauja produkta\n";
                    std::cout << "[2] Prideti nauja kategorija\n";
                    std::cout << "[3] Prideti nauja klienta\n";
                    std::cout << "[4] Prideti nauja uzsakyma\n";
                    std::cout << "[5] Prideti nauja sandeli\n";
                    std::cout << "[6] Prideti nauja darbuotoja\n";
                    std::cout << "[7] Prideti nauja tiekeja\n";
                    std::cout << "[8] Prideti nauja specifikacija\n";
                    std::cout << "[0] Grizti\n";

                    int choiceAdd;
                    std::cin >> choiceAdd;

                    switch (choiceAdd) {
                        case 1:
                            addProduct();
                            break;
                        case 2:
                            addCategory();
                            break;
                        case 3:
                            addCustomer();
                            break;
                        case 4:
                            addOrder();
                            break;
                        case 5:
                            addWarehouse();
                            break;
                        case 6:
                            addEmployee();
                            break;
                        case 7:
                            addSupplier();
                            break;
                        case 8:
                            addSpecification();
                            break;
                        case 0:
                            std::cout << "Grizti\n";
                            break;
                        default:
                            std::cout << "Neteisingas pasirinkimas\n";
                            break;
                    }
                    if (choiceAdd == 0) {
                        break;
                    }
                }
                break;
            case 3:
                std::cout << "Duomenu atnaujinimas\n";
                
                while (true) {
                    std::cout << "-------------------\n";
                    std::cout << "[1] Pakeisti produkto kaina\n";
                    std::cout << "[2] Pakeisti produkto kieki\n";
                    std::cout << "[3] Pakeisti pakeisti produkto kieki uzsakyme\n";
                    std::cout << "[4] Prideti produkta prie uzsakymo\n";
                    std::cout << "[5] Ismesti darbuotojo is darbo\n";
                    std::cout << "[6] Pakeisti vartotojo varda\n";
                    std::cout << "[7] Pakeisti produkto specifikacija\n";
                    std::cout << "[8] Pakeisti tiekejo pristatymo kaina\n";
                    std::cout << "[0] Grizti\n";

                    int choiceUpdate;
                    std::cin >> choiceUpdate;

                    switch (choiceUpdate) {
                        case 1:
                            updateProductPrice();
                            break;
                        case 2:
                            updateProductQuantity(); // FIX warehouse stock wronk
                            break;
                        case 3:
                            std::cout << "Prideti nauja klienta\n";
                            break;
                        case 4:
                            std::cout << "Prideti nauja uzsakyma\n";
                            break;
                        case 5:
                            std::cout << "Prideti nauja sandeli\n";
                            break;
                        case 6:
                            std::cout << "Prideti nauja darbuotoja\n";
                            break;
                        case 7:
                            std::cout << "Prideti nauja tiekeja\n";
                            break;
                        case 8:
                            std::cout << "Prideti nauja specifikacija\n";
                            break;
                        case 0:
                            std::cout << "Grizti\n";
                            break;
                        default:
                            std::cout << "Neteisingas pasirinkimas\n";
                            break;
                    }
                    if (choiceUpdate == 0) {
                        break;
                    }
                }

                break;
            case 4:
                std::cout << "Duomenu trinimas\n";
                while (true) {
                    std::cout << "-------------------\n";
                    std::cout << "[1] Istrinti produkta\n";
                    std::cout << "[2] Istrinti kategorija\n";
                    std::cout << "[3] Istrinti klienta\n";
                    std::cout << "[4] Istrinti uzsakyma\n";
                    std::cout << "[5] Istrinti sandeli\n";
                    std::cout << "[6] Istrinti darbuotoja\n";
                    std::cout << "[0] Grizti\n";

                    int choiceAdd;
                    std::cin >> choiceAdd;

                    switch (choiceAdd) {
                        case 1:
                            removeProduct();
                            break;
                        case 2:
                            removeCategory();
                            break;
                        case 3:
                            removeCustomer();
                            break;
                        case 4:
                            removeOrder();
                            break;
                        case 5:
                            removeWarehouse();
                            break;
                        case 6:
                            removeEmployee();
                            break;
                        case 0:
                            std::cout << "Grizti\n";
                            break;
                        default:
                            std::cout << "Neteisingas pasirinkimas\n";
                            break;
                    }
                    if (choiceAdd == 0) {
                        break;
                    }
                }
                break;
            case 0:
                std::cout << "Isjungti\n";
                return 0;
            default:
                std::cout << "Neteisingas pasirinkimas\n";
                break;
        }
    }

    return 0;
}