#include <iostream>
#include <stdio.h>
#include <string.h>
#include <conio.h>

#define MAX_PASSWORD_LENGTH 100

char* getpass(const char* prompt) {
    static char password[MAX_PASSWORD_LENGTH];
    std::cout << prompt;
    int i = 0;
    while (i < MAX_PASSWORD_LENGTH - 1) {
        char ch = _getch();
        if (ch == '\r' || ch == '\n') {
            break;
        } else if (ch == '\b' && i > 0) {
            std::cout << "\b \b";
            i--;
        } else {
            password[i++] = ch;
            std::cout << '*';
        }
    }
    password[i] = '\0';
    std::cout << std::endl;
    return password;
}

EXEC SQL BEGIN DECLARE SECTION;
int result;
char password[MAX_PASSWORD_LENGTH];
char username[9] = "username";
EXEC SQL END DECLARE SECTION;

int main() {
    printf("Programa pradeda darba\n");

    char* pwd = getpass("Iveskite slaptazodi: ");
    strncpy(password, pwd, sizeof(password) - 1);
    password[sizeof(password) - 1] = '\0';

    EXEC SQL CONNECT TO studentu@localhost:5432 USER :username USING :password;

    printf("CONNECT TO biblio. SQLCODE=%ld\n", SQLCODE);
    if (0 == SQLCODE) {
        EXEC SQL SELECT COUNT(*) INTO :result FROM username.product;
    }
    if (0 == SQLCODE) {
        printf("Programa veikia! Produktu skaicius=%d\n", result);
    } else {
        printf("SQLCODE=%ld\n", SQLCODE);
    }

    EXEC SQL DISCONNECT;

    while (true) {
        std::cout << "[1] Duomenu paieska\n";
        std::cout << "[2] Duomenu ivedimas\n";
        std::cout << "[3] Duomenu atnaujinimas\n";
        std::cout << "[4] Duomenu trynimas\n";
        std::cout << "[5] Isjungti\n";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Duomenu paieska\n";
                break;
            case 2:
                std::cout << "Duomenu ivedimas\n";
                std::cout << "-------------------\n";
                break;
            case 3:
                std::cout << "Duomenu atnaujinimas\n";
                break;
            case 4:
                std::cout << "Duomenu trynimas\n";
                break;
            case 5:
                std::cout << "Isjungti\n";
                return 0;
            default:
                std::cout << "Neteisingas pasirinkimas\n";
                break;
        }
    }

    return 0;
}