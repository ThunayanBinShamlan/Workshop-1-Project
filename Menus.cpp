#include <iostream>
#include "Menus.h"
#include "Functions.h"

using namespace std;



///////////////////////////	  MAIN MENU      ///////////////////////

void showMainMenu() {
    int choice;

    do {
        clearScreen();

        cout << "\n====================================\n";
        cout << "        CAR RENTAL SYSTEM\n";
        cout << "====================================\n";
        cout << "1. Login\n";
        cout << "2. Register Customer\n";
        cout << "0. Exit\n";
        cout << "Choose option: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            pauseScreen();
            continue;
        }


        if (choice == 1) {
    
            clearScreen();
            string role = login();

            if (role == "admin") {
                adminMenu();
            }
            else if (role == "staff") {
                staffMenu();
            }

            else if (role == "customer") {
                customerMenu();
            }
        }
        else if (choice == 2) {
			clearScreen();
			registerCustomer();
			pauseScreen();
        }

        else if (choice == 0) {

            cout <<  "Exiting system...\n";
        }

        else {
            cout << "Invalid choice. Please try again.\n";
			pauseScreen();
        }

    } while (choice != 0);
}


///////////////////////       ADMIN MENU        ///////////////////////

void adminMenu() {
    int choice;

    do {
        clearScreen();

        cout << "\n====================================\n";
        cout << "             ADMIN MENU\n";
        cout << "====================================\n";
        cout << "1. Manage Users\n";
        cout << "2. Manage Customers\n";
        cout << "3. Manage Cars\n";
        cout << "4. Manage Rentals\n";
        cout << "5. Reports\n";
        cout << "0. Logout\n";
        cout << "Choose option: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1:
            manageUsers();

            break;
        case 2:
            clearScreen();
			manageCustomers();
            pauseScreen();
            break;

        case 3:
            manageCars();
;
            break;

        case 4:
            manageRentals();

            break;

        case 5:
            reportsMenu();


            break;
        case 0:
            cout << "Logging out...\n";
            break;

        default:
            cout << "Invalid choice.\n";
            pauseScreen();

        }

    } while (choice != 0);
}

void staffMenu() {
    int choice;

    do {
        clearScreen();

        cout << "\n=====================================\n";
        cout << "             STAFF MENU\n";
        cout << "=====================================\n";
        cout << "1. Manage Customers\n";
        cout << "2. Manage Cars\n";
        cout << "3. Manage Rentals\n";
        cout << "4. View Available Cars\n";
        cout << "0. Logout\n";
        cout << "Choose option: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1:
            clearScreen();
            manageCustomers();
            break;

        case 2:
            clearScreen();
            manageCars();
            break;

        case 3:
            clearScreen();
            manageRentals();
            break;

        case 4:
            clearScreen();
            viewAvailableCars();
            pauseScreen();
            break;

        case 0:
            cout << "Logging out...\n";
            break;

        default:
            cout << "Invalid choice.\n";
            pauseScreen();
        }

    } while (choice != 0);
}

void customerMenu() {
    int choice;

    do {
        clearScreen();

        cout << "\n====================================\n";
        cout << "           CUSTOMER MENU\n";
        cout << "====================================\n";
        cout << "Welcome, " << getCustomerName() << "\n\n";
        cout << "1. View Available Cars\n";
        cout << "2. Rent Car\n";
        cout << "3. Rental History\n";
        cout << "4. View My Invoices\n";
        cout << "5. My Profile\n";
        cout << "0. Logout\n";
        cout << "Choose option: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1:
            clearScreen();
            viewAvailableCars();
            pauseScreen();
            break;

        case 2:
            clearScreen();
            rentCar();
            pauseScreen();
            break;

        case 3:
            clearScreen();
            viewRentalHistory();
            pauseScreen();
            break;

        case 4:
            clearScreen();
            viewMyInvoices();
            pauseScreen();
            break;

        case 5:
            clearScreen();
            myProfile();
            break;

        case 0:
            cout << "Logging out...\n";
            currentCustomerID = 0;
            currentUserID = 0;
            break;

        default:
            cout << "Invalid choice.\n";
            pauseScreen();
        }

    } while (choice != 0);
}

