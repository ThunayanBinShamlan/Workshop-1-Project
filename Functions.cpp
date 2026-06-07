#include <iostream>
#include <iomanip>
#include <conio.h>
#include <fstream>
#include "Functions.h"
#include "Database.h"

using namespace std;


////////////////////////////////////	 LOGIN FUNCTION      ///////////////////////

string login() 
{
    string username, password, status;

	cout << "\n=======================================\n";
	cout << "                LOGIN\n";
	cout << "=========================================\n";

	cout << "Username: ";
	cin >> username;

	cout << "Password: ";
	password = hiddenPassword();

    string query =
        "SELECT role FROM USERS WHERE username = '" + username +
        "' AND password = '" + password +
        "' AND status = 'active';";


	string role = executeQuery(query);

    if (role.empty()) {
        string statusQuery =
            "SELECT status FROM USERS WHERE username = '" + username +
            "' AND password = '" + password + "';";

        string status = executeQuery(statusQuery);

         if (status == "inactive") {
			 cout << "Your account is inactive. Please contact the administrator.\n";
	}
         else {
            cout << "Invalid username or password.\n";
        }
        return "";
	}

	cout << "Login successful.\n";

    currentUserID = stoi(executeQuery(
        "SELECT user_id FROM USERS WHERE username = '" + username + "';"
    ));

    if (role == "customer") {
        currentCustomerID = executeQuery(
            "SELECT customer_id FROM CUSTOMER WHERE user_id = " + to_string(currentUserID) + ";"
        );
    }

	return role;
}

///////////////////////////////////	 REGISTER FUNCTION      ///////////////////////

void registerCustomer()
{

    string username, password;
    string id, name, phone, address, license;

    cout << "\n============================= REGISTER =============================\n";

    cout << "Username: ";
    cin >> username;

    cout << "Password: ";
    password = hiddenPassword();

    cout << "National ID: ";
    cin >> id;

    cout << "Full Name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Phone Number: ";
    cin >> phone;

    cout << "Address: ";
    cin.ignore();
    getline(cin, address);

    cout << "License Number: ";
    cin >> license;

    // 1. Add user
    string query1 =
        "INSERT INTO USERS (username, password, role) VALUES ('" +
        username + "', '" + password + "', 'customer');";

    executeInstruction(query1);

    // 2. Get user_id
    string query2 =
        "SELECT user_id FROM USERS WHERE username = '" + username + "';";

    string user_id = executeQuery(query2);

    // 3. Add customer
    string query3 =
        "INSERT INTO CUSTOMER (customer_id, full_name, phone_number, address, license_number, user_id) VALUES ('" +
        id + "', '" + name + "', '" + phone + "', '" + address + "', '" + license + "', " + user_id + ");";

    executeInstruction(query3);

    cout << "Registration successful!\n";
    pauseScreen();
}

/////////////////////////////////	 HIDDEN PASSWORD FUNCTION      ///////////////////////
string hiddenPassword() {
    string password;
    char ch;

    while ((ch = _getch()) != '\r') { // Enter key
        if (ch == '\b') { // Backspace key
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b"; // Erase the last character
            }
        } 
        else {
            password += ch;
            cout << '*'; // Print asterisk for each character
        }
    }
    cout << endl; // Move to the next line after input
    return password;
}

////////////////////////////////////	 SCREEN UTILITIES      ///////////////////////
void clearScreen() {
    system("cls");
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(1000, '\n');
    cin.get();
}


//////////////////////////////// Calculation functions      ///////////////////////
double calculateRentalPrice(double pricePerDay, int days) {
    return pricePerDay * days;
}

double calculateLateFine(double pricePerDay, int lateDays) {
	const double fineRate = 0.20;// 20% of the daily price per late day
    if (lateDays <= 0) {
        return 0;
	}
    return pricePerDay * lateDays * fineRate;
}


//////////////////////////////	  CAR MANAGEMENT FUNCTIONS      ///////////////////////


///////////////////////////////      MANAGE CARS      ///////////////////////

    void manageCars() {
        int choice;


        do {
            clearScreen();

            cout << "\n====================================\n";
            cout << "          MANAGE CARS\n";
            cout << "====================================\n";
            cout << "1. Add Car\n";
            cout << "2. View Cars\n";
            cout << "3. Update Car\n";
            cout << "4. Deactivate Car\n";
           // cout << "5. View Available Cars\n";
            cout << "5. Back\n";
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
                addCar();

                break;

            case 2:
                clearScreen();
                viewCars();
				pauseScreen();
                break;

            case 3:
                updateCar();

                break;

            case 4:
                deactivateCar();

                break;

           // case 5:
             //   viewAvailableCars();
               // break;

            case 5:
                cout << "Returning to menu...\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
                pauseScreen();
            }

        } while (choice != 5);
    }

//////////////////////////      ADD CAR      ///////////////////////


    void addCar() {
        string brand, model, color, plateNumber, branchID;
        int year;
        double pricePerDay;

        cout << "\n================= ADD CAR ================\n";

        cout << "Brand: ";
        cin >> brand;

        cout << "Model: ";
        cin >> model;

        cout << "Year: ";
        cin >> year;

        cout << "Color: ";
        cin >> color;

        cout << "Price per day: ";
        cin >> pricePerDay;

        cout << "Plate number: ";
        cin >> plateNumber;

        cout << "Branch ID: ";
        cin >> branchID;


        if (year <= 0 || pricePerDay <= 0) {
            cout << "Invalid year or price.\n";
            return;
        }

        string query =
            "INSERT INTO CARS (brand, model, year, color, price_per_day, plate_number, status, branch_id) "
            "VALUES ('" + brand + "', '" + model + "', " + to_string(year) + ", '" + color + "', " +
            to_string(pricePerDay) + ", '" + plateNumber + "', 'available', '" + branchID + "');";

        executeInstruction(query);

        cout << "Car added successfully.\n";
    }


	/////////////////////////////      VIEW CARS      ///////////////////////

    void viewCars() {
        cout << "\n================ VIEW CARS ================\n\n";

        try {
            sql::Statement* stmt = globalCon->createStatement();

            string query =
                "SELECT car_id, brand, model, year, color, price_per_day, "
                "plate_number, status, branch_id FROM CARS;";

            sql::ResultSet* res = stmt->executeQuery(query);

            cout << left
                << setw(8) << "ID"
                << setw(15) << "Brand"
                << setw(15) << "Model"
                << setw(8) << "Year"
                << setw(12) << "Color"
                << setw(12) << "Price"
                << setw(15) << "Plate"
                << setw(12) << "Status"
                << setw(10) << "Branch"
                << endl;

            cout << string(107, '-') << endl;

            while (res->next()) {
                cout << left
                    << setw(8) << res->getString("car_id")
                    << setw(15) << res->getString("brand")
                    << setw(15) << res->getString("model")
                    << setw(8) << res->getString("year")
                    << setw(12) << res->getString("color")
                    << setw(12) << res->getString("price_per_day")
                    << setw(15) << res->getString("plate_number")
                    << setw(12) << res->getString("status")
                    << setw(10) << res->getString("branch_id")
                    << endl;
            }

            delete res;
            delete stmt;
        }
        catch (sql::SQLException& e) {
            cout << "SQL Error: " << e.what() << endl;
        }
    }


	////////////////////////////      VIEW AVAILABLE CARS      ///////////////////////
    

    void viewAvailableCars() {
        cout << "\n============= AVAILABLE CARS =============\n\n";

        try {
            sql::Statement* stmt = globalCon->createStatement();

            string query =
                "SELECT car_id, brand, model, year, color, price_per_day, "
                "plate_number, branch_id FROM CARS WHERE status = 'available';";

            sql::ResultSet* res = stmt->executeQuery(query);

            cout << left
                << setw(8) << "ID"
                << setw(15) << "Brand"
                << setw(15) << "Model"
                << setw(8) << "Year"
                << setw(12) << "Color"
                << setw(12) << "Price"
                << setw(15) << "Plate"
                << setw(10) << "Branch"
                << endl;

            cout << string(95, '-') << endl;

            while (res->next()) {
                cout << left
                    << setw(8) << res->getString("car_id")
                    << setw(15) << res->getString("brand")
                    << setw(15) << res->getString("model")
                    << setw(8) << res->getString("year")
                    << setw(12) << res->getString("color")
                    << setw(12) << res->getString("price_per_day")
                    << setw(15) << res->getString("plate_number")
                    << setw(10) << res->getString("branch_id")
                    << endl;
            }

            delete res;
            delete stmt;
        }
        catch (sql::SQLException& e) {
            cout << "SQL Error: " << e.what() << endl;
        }
    }


	/////////////////////////////      UPDATE CAR      ///////////////////////

    void updateCar() {

        void viewCars();
        int carID;

        cout << "\n========== UPDATE CAR ==========\n";
        cout << "Enter Car ID: ";
        cin >> carID;

        string exists = executeQuery(
            "SELECT car_id FROM CARS WHERE car_id = " + to_string(carID) + ";"
        );

        if (exists.empty()) {
            cout << "Car not found.\n";
            return;
        }

        int choice;

        do {
            cout << "\n========== UPDATE CAR MENU ==========\n";
            cout << "1. Update Brand\n";
            cout << "2. Update Model\n";
            cout << "3. Update Year\n";
            cout << "4. Update Color\n";
            cout << "5. Update Price Per Day\n";
            cout << "6. Update Plate Number\n";
            cout << "7. Update Status\n";
            cout << "8. Update Branch ID\n";
            cout << "9. Back\n";
            cout << "Choose option: ";
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Please enter a number.\n";
                continue;
			}

            string query;

            switch (choice) {
            case 1: {
                string brand;
                cout << "Enter new brand: ";
                cin >> brand;

                query = "UPDATE CARS SET brand = '" + brand +
                    "' WHERE car_id = " + to_string(carID) + ";";

                executeInstruction(query);
                cout << "Brand updated successfully.\n";
                break;
            }

            case 2: {
                string model;
                cout << "Enter new model: ";
                cin >> model;

                query = "UPDATE CARS SET model = '" + model +
                    "' WHERE car_id = " + to_string(carID) + ";";

                executeInstruction(query);
                cout << "Model updated successfully.\n";
                break;
            }

            case 3: {
                int year;
                cout << "Enter new year: ";
                cin >> year;

                if (year <= 0) {
                    cout << "Invalid year.\n";
                    break;
                }

                query = "UPDATE CARS SET year = " + to_string(year) +
                    " WHERE car_id = " + to_string(carID) + ";";

                executeInstruction(query);
                cout << "Year updated successfully.\n";
                break;
            }

            case 4: {
                string color;
                cout << "Enter new color: ";
                cin >> color;

                query = "UPDATE CARS SET color = '" + color +
                    "' WHERE car_id = " + to_string(carID) + ";";

                executeInstruction(query);
                cout << "Color updated successfully.\n";
                break;
            }

            case 5: {
                double price;
                cout << "Enter new price per day: ";
                cin >> price;

                if (price <= 0) {
                    cout << "Invalid price.\n";
                    break;
                }

                query = "UPDATE CARS SET price_per_day = " + to_string(price) +
                    " WHERE car_id = " + to_string(carID) + ";";

                executeInstruction(query);
                cout << "Price updated successfully.\n";
                break;
            }

            case 6: {
                string plateNumber;
                cout << "Enter new plate number: ";
                cin >> plateNumber;

                query = "UPDATE CARS SET plate_number = '" + plateNumber +
                    "' WHERE car_id = " + to_string(carID) + ";";

                executeInstruction(query);
                cout << "Plate number updated successfully.\n";
                break;
            }

            case 7: {
                string status;
                cout << "Enter new status (available/rented/inactive): ";
                cin >> status;

                if (status != "available" && status != "rented" && status != "inactive") {
                    cout << "Invalid status. Use available, rented, or inactive.\n";
                    break;
                }

                query = "UPDATE CARS SET status = '" + status +
                    "' WHERE car_id = " + to_string(carID) + ";";

                executeInstruction(query);
                cout << "Status updated successfully.\n";
                break;
            }

            case 8: {
                int branchID;
                cout << "Enter new Branch ID: ";
                cin >> branchID;

                string branchExists = executeQuery(
                    "SELECT branch_id FROM BRANCH WHERE branch_id = " + to_string(branchID) + ";"
                );

                if (branchExists.empty()) {
                    cout << "Branch not found.\n";
                    break;
                }

                query = "UPDATE CARS SET branch_id = " + to_string(branchID) +
                    " WHERE car_id = " + to_string(carID) + ";";

                executeInstruction(query);
                cout << "Branch updated successfully.\n";
                break;
            }

            case 9:
                cout << "Returning to Manage Cars menu...\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
            }

        } while (choice != 9);
    }


	/////////////////////////////      Deactivate CAR      ///////////////////////

     void deactivateCar() {
        int carID;

        cout << "\n========== DEACTIVATE CAR ==========\n";
        cout << "Enter Car ID: ";
        cin >> carID;

        string exists = executeQuery(
            "SELECT car_id FROM CARS WHERE car_id = " + to_string(carID) + ";"
        );

        if (exists.empty()) {
            cout << "Car not found.\n";
            return;
        }

        string status = executeQuery(
            "SELECT status FROM CARS WHERE car_id = " + to_string(carID) + ";"
        );

        if (status == "rented") {
            cout << "Cannot deactivate this car because it is currently rented.\n";
            return;
        }

        if (status == "inactive") {
            cout << "This car is already inactive.\n";
            return;
        }

        int confirm;
        cout << "Are you sure you want to deactivate this car? (1 = Yes, 0 = No): ";
        cin >> confirm;

        if (confirm != 1) {
            cout << "Operation cancelled.\n";
            return;
        }

        string query =
            "UPDATE CARS SET status = 'inactive' WHERE car_id = " + to_string(carID) + ";";

        executeInstruction(query);

        cout << "Car deactivated successfully. It will no longer be available for rental.\n";
    }


//-----------------------------------------------------------------------------------------//
////////////////////////////      CUSTOMER MANAGEMENT FUNCTIONS      ///////////////////////
//---------------------------------------------------------------------------------------//


///////////////////////////      MANAGE CUSTOMERS      ///////////////////////
    void manageCustomers() {
        int choice;

        do {
            clearScreen();

			cout << "\n====================================\n";
			cout << "          MANAGE CUSTOMERS\n";
			cout << "======================================\n";
			cout << "1. Add Customer\n";
			cout << "2. View Customers\n";
			cout << "3. Update Customer\n";
			cout << "4. Deactivate Customer\n";
			cout << "5. Back\n";
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
				addCustomer();
				pauseScreen();
                break;

            case 2:
				clearScreen();
                viewCustomers();
				pauseScreen();
				break;

            case 3:
				clearScreen();
                updateCustomer();
				pauseScreen();
                break;

            case 4:
				clearScreen();
                deactivateCustomer();
				pauseScreen();
				break;

            case 5:
				cout << "Returning to menu...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                pauseScreen();

            }
        } while (choice != 5);
	}


/////////////////////	  ADD CUSTOMER      ///////////////////////
    void addCustomer() {
        string customerID, fullName, phone, address, license;

        cout << "\n========== ADD CUSTOMER ==========\n";

        cout << "Customer ID (National ID/Passport): ";
        cin >> customerID;

        string exists = executeQuery(
            "SELECT customer_id FROM CUSTOMER WHERE customer_id = '" + customerID + "';"
        );

        if (!exists.empty()) {
            cout << "Customer already exists.\n";
            return;
        }

        cin.ignore(1000, '\n');

        cout << "Full Name: ";
        getline(cin, fullName);

        cout << "Phone Number: ";
        cin >> phone;

        cin.ignore(1000, '\n');

        cout << "Address: ";
        getline(cin, address);

        cout << "License Number: ";
        cin >> license;

        string query =
            "INSERT INTO CUSTOMER (customer_id, full_name, phone_number, address, license_number) "
            "VALUES ('" + customerID + "', '" + fullName + "', '" + phone + "', '" + address + "', '" + license + "');";

        executeInstruction(query);

        cout << "Customer added successfully.\n";
    }

//////////////////////  VIEW CUSTOMERS      ///////////////////////

    void viewCustomers() {
            cout << "\n==================== VIEW CUSTOMERS ====================\n\n";

            try {
                sql::Statement* stmt = globalCon->createStatement();

                string query =
                    "SELECT customer_id, full_name, phone_number, address, "
                    "license_number, status FROM CUSTOMER;";

                sql::ResultSet* res = stmt->executeQuery(query);

                cout << left
                    << setw(15) << "Customer ID"
                    << setw(25) << "Full Name"
                    << setw(18) << "Phone"
                    << setw(20) << "Address"
                    << setw(18) << "License"
                    << setw(12) << "Status"
                    << endl;

                cout << string(108, '-') << endl;

                while (res->next()) {

                    cout << left
                        << setw(15) << res->getString("customer_id")
                        << setw(25) << res->getString("full_name")
                        << setw(18) << res->getString("phone_number")
                        << setw(20) << res->getString("address")
                        << setw(18) << res->getString("license_number")
                        << setw(12) << res->getString("status")
                        << endl;
                }

                delete res;
                delete stmt;
            }
            catch (sql::SQLException& e) {

                cout << "SQL Error: "
                    << e.what()
                    << endl;
            }
        }

/////////////////////// UPDATE CUSTOMER data     ///////////////////////

    void updateCustomer() {
        string customerID;

        cout << "\n========== UPDATE CUSTOMER ==========\n";
        cout << "Enter Customer ID: ";
        cin >> customerID;

        string exists = executeQuery(
            "SELECT customer_id FROM CUSTOMER WHERE customer_id = '" + customerID + "';"
        );

        if (exists.empty()) {
            cout << "Customer not found.\n";
            return;
        }

        int choice;

        do {
            cout << "\n========== UPDATE CUSTOMER MENU ==========\n";
            cout << "1. Update Full Name\n";
            cout << "2. Update Phone Number\n";
            cout << "3. Update Address\n";
            cout << "4. Update License Number\n";
            cout << "5. Back\n";
            cout << "Choose option: ";
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            string query;

            switch (choice) {
            case 1: {
                string name;
                cin.ignore(1000, '\n');
                cout << "Enter new full name: ";
                getline(cin, name);

                query = "UPDATE CUSTOMER SET full_name = '" + name +
                    "' WHERE customer_id = '" + customerID + "';";

                executeInstruction(query);
                cout << "Full name updated successfully.\n";
                break;
            }

            case 2: {
                string phone;
                cout << "Enter new phone number: ";
                cin >> phone;

                query = "UPDATE CUSTOMER SET phone_number = '" + phone +
                    "' WHERE customer_id = '" + customerID + "';";

                executeInstruction(query);
                cout << "Phone number updated successfully.\n";
                break;
            }

            case 3: {
                string address;
                cin.ignore(1000, '\n');
                cout << "Enter new address: ";
                getline(cin, address);

                query = "UPDATE CUSTOMER SET address = '" + address +
                    "' WHERE customer_id = '" + customerID + "';";

                executeInstruction(query);
                cout << "Address updated successfully.\n";
                break;
            }

            case 4: {
                string license;
                cout << "Enter new license number: ";
                cin >> license;

                query = "UPDATE CUSTOMER SET license_number = '" + license +
                    "' WHERE customer_id = '" + customerID + "';";

                executeInstruction(query);
                cout << "License number updated successfully.\n";
                break;
            }

            case 5:
                cout << "Returning to Manage Customers menu...\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
            }

        } while (choice != 5);
    }

///////////////  deactivate customer      ///////////////////////

    void deactivateCustomer() {
        string customerID;

        cout << "\n========== DEACTIVATE CUSTOMER ==========\n";
        cout << "Enter Customer ID: ";
        cin >> customerID;

        string exists = executeQuery(
            "SELECT customer_id FROM CUSTOMER WHERE customer_id = '" + customerID + "';"
        );

        if (exists.empty()) {
            cout << "Customer not found.\n";
            return;
        }

        string status = executeQuery(
            "SELECT status FROM CUSTOMER WHERE customer_id = '" + customerID + "';"
        );

        if (status == "inactive") {
            cout << "Customer is already inactive.\n";
            return;
        }

        int confirm;

        cout << "Are you sure you want to deactivate this customer? (1 = Yes, 0 = No): ";
        cin >> confirm;

        if (confirm != 1) {
            cout << "Operation cancelled.\n";
            return;
        }

        string query =
            "UPDATE CUSTOMER SET status = 'inactive' "
            "WHERE customer_id = '" + customerID + "';";

        executeInstruction(query);

        cout << "Customer deactivated successfully.\n";
    }


/////////////////////// get customer name       ///////////////////////

    string getCustomerName() {
        return executeQuery(
            "SELECT full_name FROM CUSTOMER WHERE customer_id = '" + currentCustomerID + "';"
        );
    }

////////////////////  customer profile  ///////////////////////

    void myProfile() {
        int choice;

        do {
            clearScreen();

            cout << "\n========== MY PROFILE ==========\n\n";

            string customerID = currentCustomerID;

            cout << "Customer ID     : " << customerID << endl;
            cout << "Full Name       : " << executeQuery("SELECT full_name FROM CUSTOMER WHERE customer_id = '" + customerID + "';") << endl;
            cout << "Phone Number    : " << executeQuery("SELECT phone_number FROM CUSTOMER WHERE customer_id = '" + customerID + "';") << endl;
            cout << "Address         : " << executeQuery("SELECT address FROM CUSTOMER WHERE customer_id = '" + customerID + "';") << endl;
            cout << "License Number  : " << executeQuery("SELECT license_number FROM CUSTOMER WHERE customer_id = '" + customerID + "';") << endl;
            cout << "Status          : " << executeQuery("SELECT status FROM CUSTOMER WHERE customer_id = '" + customerID + "';") << endl;

            cout << "\n--------------------------------\n";
            cout << "1. Update Full Name\n";
            cout << "2. Update Phone Number\n";
            cout << "3. Update Address\n";
            cout << "4. Update License Number\n";
            cout << "5. Back\n";
            cout << "Choose option: ";
            cin >> choice;

            string query;

            switch (choice) {
            case 1: {
                string name;
                cin.ignore(1000, '\n');
                cout << "Enter new full name: ";
                getline(cin, name);

                query = "UPDATE CUSTOMER SET full_name = '" + name +
                    "' WHERE customer_id = '" + customerID + "';";
                executeInstruction(query);
                cout << "Full name updated successfully.\n";
                pauseScreen();
                break;
            }

            case 2: {
                string phone;
                cout << "Enter new phone number: ";
                cin >> phone;

                query = "UPDATE CUSTOMER SET phone_number = '" + phone +
                    "' WHERE customer_id = '" + customerID + "';";
                executeInstruction(query);
                cout << "Phone number updated successfully.\n";
                pauseScreen();
                break;
            }

            case 3: {
                string address;
                cin.ignore(1000, '\n');
                cout << "Enter new address: ";
                getline(cin, address);

                query = "UPDATE CUSTOMER SET address = '" + address +
                    "' WHERE customer_id = '" + customerID + "';";
                executeInstruction(query);
                cout << "Address updated successfully.\n";
                pauseScreen();
                break;
            }

            case 4: {
                string license;
                cout << "Enter new license number: ";
                cin >> license;

                query = "UPDATE CUSTOMER SET license_number = '" + license +
                    "' WHERE customer_id = '" + customerID + "';";
                executeInstruction(query);
                cout << "License number updated successfully.\n";
                pauseScreen();
                break;
            }

            case 5:
                cout << "Returning...\n";
                break;

            default:
                cout << "Invalid choice.\n";
                pauseScreen();
            }

        } while (choice != 5);
    }

///////////////////  view customer rental history      ///////////////////////

    void viewRentalHistory() {
        cout << "\n==================== MY RENTAL HISTORY ====================\n\n";

        try {
            sql::Statement* stmt = globalCon->createStatement();

            string query =
                "SELECT R.rental_id, C.brand, C.model, R.rental_date, R.return_date, "
                "R.days, R.total_price, R.rental_status "
                "FROM RENTALS R "
                "JOIN CARS C ON R.car_id = C.car_id "
                "WHERE R.customer_id = '" + currentCustomerID + "' "
                "ORDER BY R.rental_date DESC;";

            sql::ResultSet* res = stmt->executeQuery(query);

            cout << left
                << setw(12) << "Rental ID"
                << setw(22) << "Car"
                << setw(15) << "Rent Date"
                << setw(15) << "Return Date"
                << setw(8) << "Days"
                << setw(15) << "Price"
                << setw(15) << "Status"
                << endl;

            cout << string(102, '-') << endl;

            bool hasData = false;

            while (res->next()) {
                hasData = true;

                string carName = res->getString("brand") + " " + res->getString("model");

                cout << left
                    << setw(12) << res->getString("rental_id")
                    << setw(22) << carName
                    << setw(15) << res->getString("rental_date")
                    << setw(15) << res->getString("return_date")
                    << setw(8) << res->getString("days")
                    << setw(15) << res->getString("total_price")
                    << setw(15) << res->getString("rental_status")
                    << endl;
            }

            if (!hasData) {
                cout << "No rental history found.\n";
            }

            delete res;
            delete stmt;
        }
        catch (sql::SQLException& e) {
            cout << "SQL Error: " << e.what() << endl;
        }
    }

	///////////// show customer rental info      ///////////////////////
    void showCustomerRentalInfo() {
        try {
            sql::Statement* stmt = globalCon->createStatement();

            string query =
                "SELECT customer_id, full_name, phone_number, license_number "
                "FROM CUSTOMER WHERE customer_id = '" + currentCustomerID + "';";

            sql::ResultSet* res = stmt->executeQuery(query);

            cout << "\n========== CUSTOMER INFORMATION ==========\n";

            if (res->next()) {
                cout << left << setw(18) << "Customer ID" << ": " << res->getString("customer_id") << endl;
                cout << left << setw(18) << "Full Name" << ": " << res->getString("full_name") << endl;
                cout << left << setw(18) << "Phone Number" << ": " << res->getString("phone_number") << endl;
                cout << left << setw(18) << "License Number" << ": " << res->getString("license_number") << endl;
            }
            else {
                cout << "Customer information not found.\n";
            }

            cout << "==========================================\n";

            delete res;
            delete stmt;
        }
        catch (sql::SQLException& e) {
            cout << "SQL Error: " << e.what() << endl;
        }
    }


	//-----------------------------------------------------------------------------------------//
	//----------------------------      USER MANAGEMENT FUNCTIONS      -------------------------//
	//-----------------------------------------------------------------------------------------//

    void manageUsers() {
        int choice;

        do {
            clearScreen();

            cout << "\n====================================\n";
            cout << "           MANAGE USERS\n";
            cout << "====================================\n";
            cout << "1. Add User\n";
            cout << "2. View Users\n";
            cout << "3. Update User Role\n";
            cout << "4. Deactivate User\n";
            cout << "5. Back\n";
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
                addUser();
                pauseScreen();
                break;

            case 2:
                clearScreen();
                viewUsers();
                pauseScreen();
                break;

            case 3:
                clearScreen();
                updateUser();
                pauseScreen();
                break;

            case 4:
                clearScreen();
                deactivateUser();
                pauseScreen();
                break;

            case 5:
                cout << "Returning...\n";
                break;

            default:
                cout << "Invalid choice.\n";
                pauseScreen();
            }

        } while (choice != 5);
    }


	////////////////////////      Add Users      ///////////////////////

    void addUser() {

        string username, password, role;

        cout << "\n========== ADD USER ==========\n";

        cout << "Username: ";
        cin >> username;

        string exists = executeQuery(
            "SELECT user_id FROM USERS WHERE username = '" + username + "';"
        );

        if (!exists.empty()) {
            cout << "Username already exists.\n";
            return;
        }

        cout << "Password: ";
        password = hiddenPassword();

        cout << "Role (admin/staff): ";
        cin >> role;

        if (role != "admin" && role != "staff") {
            cout << "Invalid role. Admin can only add admin or staff.\n";
            return;
        }

        string query =
            "INSERT INTO USERS (username, password, role) VALUES ('" +
            username + "', '" + password + "', '" + role + "');";

        executeInstruction(query);

        cout << "User added successfully.\n";
    }


	//////////////////////////      View Users      ///////////////////////

    void viewUsers() {
        cout << "\n==================== VIEW USERS ====================\n\n";
        try {
            sql::Statement* stmt = globalCon->createStatement();

            string query =
                "SELECT user_id, username, role, status FROM USERS;";
            sql::ResultSet* res = stmt->executeQuery(query);

            cout << left
                << setw(10) << "User ID"
                << setw(20) << "Username"
                << setw(15) << "Role"
				<< setw(15) << "Status"
                << endl;

            cout << string(45, '-') << endl;
            while (res->next()) {
                cout << left
                    << setw(10) << res->getString("user_id")
                    << setw(20) << res->getString("username")
                    << setw(15) << res->getString("role")
                    << setw(15) << res->getString("status")
                    << endl;
            }

            delete res;
            delete stmt;
        }
        catch (sql::SQLException& e) {
            cout << "SQL Error: " << e.what() << endl;
        }
	}

	//////////////////// 	Update User     ///////////////////////

    void updateUser() {
        int userID;

        cout << "\n========== UPDATE USER ==========\n";
        cout << "Enter User ID: ";
        cin >> userID;

        string exists = executeQuery(
            "SELECT user_id FROM USERS WHERE user_id = " + to_string(userID) + ";"
        );

        if (exists.empty()) {
            cout << "User not found.\n";
            return;
        }

        int choice;

        do {
            cout << "\n========== UPDATE USER MENU ==========\n";
            cout << "1. Update Username\n";
            cout << "2. Update Password\n";
            cout << "3. Update Role\n";
            cout << "4. Back\n";
            cout << "Choose option: ";
            cin >> choice;

            string query;

            switch (choice) {
            case 1: {
                string username;

                cout << "Enter new username: ";
                cin >> username;

                string usernameExists = executeQuery(
                    "SELECT user_id FROM USERS WHERE username = '" + username + "';"
                );

                if (!usernameExists.empty()) {
                    cout << "Username already exists.\n";
                    break;
                }

                query =
                    "UPDATE USERS SET username = '" + username +
                    "' WHERE user_id = " + to_string(userID) + ";";

                executeInstruction(query);
                cout << "Username updated successfully.\n";
                break;
            }

            case 2: {
                string password;

                cout << "Enter new password: ";
                password = hiddenPassword();

                query =
                    "UPDATE USERS SET password = '" + password +
                    "' WHERE user_id = " + to_string(userID) + ";";

                executeInstruction(query);
                cout << "Password updated successfully.\n";
                break;
            }

            case 3: {
                string role;

                cout << "Enter new role (admin/staff/customer): ";
                cin >> role;

                if (role != "admin" && role != "staff" && role != "customer") {
                    cout << "Invalid role.\n";
                    break;
                }

                query =
                    "UPDATE USERS SET role = '" + role +
                    "' WHERE user_id = " + to_string(userID) + ";";

                executeInstruction(query);
                cout << "Role updated successfully.\n";
                break;
            }

            case 4:
                cout << "Returning to Manage Users menu...\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
            }

        } while (choice != 4);
    }

	/////////////////////  Deactivate User     ///////////////////////  

    void deactivateUser() {
        int userID;
        string newStatus;

        cout << "\n========== UPDATE USER STATUS ==========\n";
        cout << "Enter User ID: ";
        cin >> userID;

        string exists = executeQuery(
            "SELECT user_id FROM USERS WHERE user_id = " + to_string(userID) + ";"
        );

        if (exists.empty()) {
            cout << "User not found.\n";
            return;
        }

        cout << "Enter new status (active/inactive): ";
        cin >> newStatus;

        if (newStatus != "active" && newStatus != "inactive") {
            cout << "Invalid status. Use active or inactive.\n";
            return;
        }

        string query =
            "UPDATE USERS SET status = '" + newStatus +
            "' WHERE user_id = " + to_string(userID) + ";";

        executeInstruction(query);

        cout << "User status updated successfully.\n";
    }

	//-----------------------------------------------------------------------------------------//
	//----------------------------      RENTAL MANAGEMENT FUNCTIONS      -------------------------//    
    //-----------------------------------------------------------------------------------------//

    void manageRentals() {
        int choice;

        do {
            clearScreen();

            cout << "\n====================================\n";
            cout << "          MANAGE RENTALS\n";
            cout << "====================================\n";
            cout << "1. Create Rental\n";
            cout << "2. Return Car\n";
            cout << "3. View Rentals\n";
            cout << "4. Update Rental\n";
            cout << "5. Cancel Rental\n";
            cout << "6. View Invoices\n";
            cout << "7. View Available Cars\n";
            cout << "8. Back\n";
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
                rentCar(); 
                pauseScreen(); 
                break;

            case 2: 
                clearScreen(); 
                returnCar();
                pauseScreen();
                break;

            case 3: 
                clearScreen();
                viewRentals();
                pauseScreen();
                break;

            case 4:
                clearScreen();
                updateRental();
                pauseScreen();
                break;

            case 5:
                clearScreen();
                cancelRental();
                pauseScreen();
                break;

            case 6:
                clearScreen();
                viewInvoice();
                pauseScreen();
                break;

            case 7: 
                clearScreen();
                viewAvailableCars();
                pauseScreen();
                break;

            case 8:
                cout << "Returning...\n";
                break;

            default: cout << "Invalid choice.\n"; pauseScreen();
            }

        } while (choice != 8);
    }

    //////////////////////// Rent car ///////////////////////////

    void rentCar() {
        string customerID, paymentMethod;
        int carID, days;

        cout << "\n========== CREATE RENTAL ==========\n";

        if (!currentCustomerID.empty()) {
            customerID = currentCustomerID;
            showCustomerRentalInfo();
        }
        else {
            cout << "Enter Customer ID: ";
            cin >> customerID;
        }

        string customerExists = executeQuery(
            "SELECT customer_id FROM CUSTOMER WHERE customer_id = '" + customerID + "' AND status = 'active';"
        );

        if (customerExists.empty()) {
            cout << "Customer not found or inactive.\n";
            return;
        }

        viewAvailableCars();

        cout << "\nEnter Car ID: ";
        cin >> carID;

        string carStatus = executeQuery(
            "SELECT status FROM CARS WHERE car_id = " + to_string(carID) + ";"
        );

        if (carStatus.empty()) {
            cout << "Car not found.\n";
            return;
        }

        if (carStatus != "available") {
            cout << "Car is not available.\n";
            return;
        }

        cout << "Enter number of days: ";
        cin >> days;

        if (cin.fail() || days <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid days.\n";
            return;
        }

        string priceStr = executeQuery(
            "SELECT price_per_day FROM CARS WHERE car_id = " + to_string(carID) + ";"
        );

        double pricePerDay = stod(priceStr);
        double totalPrice = calculateRentalPrice(pricePerDay, days);

        cout << "Total Price: RM " << totalPrice << endl;

        int confirm;
        cout << "Confirm rental? (1 = Yes, 0 = No): ";
        cin >> confirm;

        if (confirm != 1) {
            cout << "Rental cancelled.\n";
            return;
        }

        cout << "Payment method (cash/card): ";
        cin >> paymentMethod;

        if (paymentMethod != "cash" && paymentMethod != "card") {
            cout << "Invalid payment method.\n";
            return;
        }

        string rentalQuery =
            "INSERT INTO RENTALS (rental_date, return_date, days, total_price, customer_id, car_id, rental_status) "
            "VALUES (CURDATE(), DATE_ADD(CURDATE(), INTERVAL " + to_string(days) +
            " DAY), " + to_string(days) + ", " + to_string(totalPrice) + ", '" +
            customerID + "', " + to_string(carID) + ", 'active');";

        executeInstruction(rentalQuery);

        string rentalID = executeQuery("SELECT LAST_INSERT_ID();");

        string invoiceQuery =
            "INSERT INTO INVOICES (amount, payment_date, payment_method, rental_id) "
            "VALUES (" + to_string(totalPrice) + ", CURDATE(), '" + paymentMethod + "', " + rentalID + ");";

        executeInstruction(invoiceQuery);

        executeInstruction(
            "UPDATE CARS SET status = 'rented' WHERE car_id = " + to_string(carID) + ";"
        );

        cout << "Rental created successfully.\n";
        cout << "Invoice generated successfully.\n";
    }


    /////////////// return car ////////////////////

    void returnCar() {
        int rentalID;

        viewActiveRentals();

        cout << "\nEnter Rental ID to return: ";
        cin >> rentalID;

        string status = executeQuery(
            "SELECT rental_status FROM RENTALS WHERE rental_id = " + to_string(rentalID) + ";"
        );

        if (status.empty()) {
            cout << "Rental not found.\n";
            return;
        }

        if (status != "active") {
            cout << "Only active rentals can be returned.\n";
            return;
        }

        string carID = executeQuery(
            "SELECT car_id FROM RENTALS WHERE rental_id = " + to_string(rentalID) + ";"
        );

        string priceStr = executeQuery(
            "SELECT C.price_per_day "
            "FROM RENTALS R "
            "JOIN CARS C ON R.car_id = C.car_id "
            "WHERE R.rental_id = " + to_string(rentalID) + ";"
        );

        string lateDaysStr = executeQuery(
            "SELECT DATEDIFF(CURDATE(), return_date) "
            "FROM RENTALS WHERE rental_id = " + to_string(rentalID) + ";"
        );

        int lateDays = stoi(lateDaysStr);

        if (lateDays < 0) {
            lateDays = 0;
        }

        double pricePerDay = stod(priceStr);
        double lateFine = calculateLateFine(pricePerDay, lateDays);

        executeInstruction(
            "UPDATE RENTALS SET rental_status = 'completed', late_fine = " +
            to_string(lateFine) +
            " WHERE rental_id = " + to_string(rentalID) + ";"
        );

        executeInstruction(
            "UPDATE INVOICES SET amount = amount + " + to_string(lateFine) +
            " WHERE rental_id = " + to_string(rentalID) + ";"
        );

        executeInstruction(
            "UPDATE CARS SET status = 'available' WHERE car_id = " + carID + ";"
        );

        cout << "\nCar returned successfully.\n";

        if (lateDays > 0) {
            cout << "Late Days : " << lateDays << endl;
            cout << "Late Fine : RM " << lateFine << endl;
        }
        else {
            cout << "No late fine applied.\n";
        }
    }

    /////////// view rental  ///////////

    void viewRentals() {

       cout << "\n==================== VIEW RENTALS ====================\n\n";

        try {

            sql::Statement* stmt = globalCon->createStatement();

            string query =
                "SELECT rental_id, rental_date, return_date, days, total_price, "
                "customer_id, car_id, rental_status "
                "FROM RENTALS;";

            sql::ResultSet* res = stmt->executeQuery(query);

            cout << left
                << setw(10) << "Rental ID"
                << setw(15) << "Rent Date"
                << setw(15) << "Return Date"
                << setw(10) << "Days"
                << setw(15) << "Total Price"
                << setw(18) << "Customer ID"
                << setw(10) << "Car ID"
                << setw(15) << "Status"
                << endl;

            cout << string(108, '-') << endl;

            while (res->next()) {

                cout << left
                    << setw(10) << res->getString("rental_id")
                    << setw(15) << res->getString("rental_date")
                    << setw(15) << res->getString("return_date")
                    << setw(10) << res->getString("days")
                    << setw(15) << res->getString("total_price")
                    << setw(18) << res->getString("customer_id")
                    << setw(10) << res->getString("car_id")
                    << setw(15) << res->getString("rental_status")
                    << endl;
            }

            delete res;
            delete stmt;
        }
        catch (sql::SQLException& e) {

            cout << "SQL Error: "
                << e.what()
                << endl;
        }
    }

    /////////////// update Rental ///////////////

    void updateRental() {

        int rentalID;

        cout << "\n========== UPDATE RENTAL ==========\n";
        cout << "Enter Rental ID: ";
        cin >> rentalID;

        string status = executeQuery(
            "SELECT rental_status FROM RENTALS WHERE rental_id = " + to_string(rentalID) + ";"
        );

        if (status.empty()) {
            cout << "Rental not found.\n";
            return;
        }

        if (status != "active") {
            cout << "Only active rentals can be updated.\n";
            return;
        }

        int newDays;

        cout << "Enter new number of days: ";
        cin >> newDays;

        if (cin.fail() || newDays <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid days.\n";
            return;
        }

        string carID = executeQuery(
            "SELECT car_id FROM RENTALS WHERE rental_id = " + to_string(rentalID) + ";"
        );

        string priceStr = executeQuery(
            "SELECT price_per_day FROM CARS WHERE car_id = " + carID + ";"
        );

        double pricePerDay = stod(priceStr);
        double totalPrice = calculateRentalPrice(pricePerDay, newDays);

        string updateRentalQuery =
            "UPDATE RENTALS SET "
            "days = " + to_string(newDays) + ", "
            "return_date = DATE_ADD(rental_date, INTERVAL " + to_string(newDays) + " DAY), "
            "total_price = " + to_string(totalPrice) +
            " WHERE rental_id = " + to_string(rentalID) + ";";

        executeInstruction(updateRentalQuery);

        string updateInvoice =
            "UPDATE INVOICES SET amount = " + to_string(totalPrice) +
            " WHERE rental_id = " + to_string(rentalID) + ";";

        executeInstruction(updateInvoice);

        cout << "Rental updated successfully.\n";
    }

    ///////////////// cancel rental //////////////

    void cancelRental() {

        int rentalID;

        cout << "\n========== CANCEL RENTAL ==========\n";
        cout << "Enter Rental ID: ";
        cin >> rentalID;

        string status = executeQuery(
            "SELECT rental_status FROM RENTALS WHERE rental_id = " + to_string(rentalID) + ";"
        );

        if (status.empty()) {
            cout << "Rental not found.\n";
            return;
        }

        if (status != "active") {
            cout << "Only active rentals can be cancelled.\n";
            return;
        }

        string carID = executeQuery(
            "SELECT car_id FROM RENTALS WHERE rental_id = " + to_string(rentalID) + ";"
        );

        executeInstruction(
            "UPDATE RENTALS SET rental_status = 'cancelled' "
            "WHERE rental_id = " + to_string(rentalID) + ";"
        );

        executeInstruction(
            "UPDATE CARS SET status = 'available' "
            "WHERE car_id = " + carID + ";"
        );

        cout << "Rental cancelled successfully.\n";
    }


    ///////////////////////////////// view active rental ////////////////////////////////
    void viewActiveRentals() {
        cout << "\n============================== ACTIVE RENTALS ==============================\n\n";

        try {
            sql::Statement* stmt = globalCon->createStatement();

            string query =
                "SELECT R.rental_id, CU.full_name, C.brand, C.model, "
                "R.rental_date, R.return_date, R.days, R.total_price "
                "FROM RENTALS R "
                "JOIN CUSTOMER CU ON R.customer_id = CU.customer_id "
                "JOIN CARS C ON R.car_id = C.car_id "
                "WHERE R.rental_status = 'active';";

            sql::ResultSet* res = stmt->executeQuery(query);

            cout << left
                << setw(12) << "Rental ID"
                << setw(25) << "Customer"
                << setw(22) << "Car"
                << setw(15) << "Rent Date"
                << setw(15) << "Return Date"
                << setw(8) << "Days"
                << setw(12) << "Price"
                << endl;

            cout << string(109, '-') << endl;

            bool hasData = false;

            while (res->next()) {
                hasData = true;

                string carName = res->getString("brand") + " " + res->getString("model");

                cout << left
                    << setw(12) << res->getString("rental_id")
                    << setw(25) << res->getString("full_name")
                    << setw(22) << carName
                    << setw(15) << res->getString("rental_date")
                    << setw(15) << res->getString("return_date")
                    << setw(8) << res->getString("days")
                    << setw(12) << res->getString("total_price")
                    << endl;
            }

            cout << string(109, '-') << endl;

            if (!hasData) {
                cout << "No active rentals found.\n";
            }

            delete res;
            delete stmt;
        }
        catch (sql::SQLException& e) {
            cout << "SQL Error: " << e.what() << endl;
        }
    }

    //-----------------------------------------------------------------------------------------//
    //----------------------------      REPORT FUNCTION  --------------------------------------//
    //-----------------------------------------------------------------------------------------//

    void reportsMenu() {
        int choice;
        do {
            clearScreen();

            cout << "\n====================================\n";
            cout << "             REPORTS\n";
            cout << "====================================\n";
            cout << "1. Generate Full System Report\n";
            cout << "2. Monthly Revenue Report\n";
            cout << "3. Yearly Revenue Report\n";
            cout << "4. Total Rentals Report\n";
            cout << "5. Most Rented Car Report\n";
            cout << "6. Back\n";
            cout << "Choose option: ";
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Please enter a number.\n";
                pauseScreen();
                continue;
            }

            switch (choice) {
            case 1:
                clearScreen();
                generateFullReport();
                pauseScreen();
                break;

            case 2:
                clearScreen();
                monthlyReport();
                pauseScreen();
                break;

            case 3:
                clearScreen();
                yearlyReport();
                pauseScreen();
                break;

            case 4:
                clearScreen();
                totalRentalsReport();
                pauseScreen();
                break;

            case 5:
                clearScreen();
                mostRentedCarReport();
                pauseScreen();
                break;

            case 6:
                cout << "Returning...\n";
                break;

            default:
                cout << "Invalid choice.\n";
                pauseScreen();
            }

        } while (choice != 6);
    }

	/////////////////////// Helper function to get report value with NULL handling  ///////////////////////
    string getReportValue(const string& query) {
        string value = executeQuery(query);

        if (value.empty() || value == "NULL") {
            return "0";
        }

        return value;
    }

	///////////////////// Helper function to print report line with proper formatting  ///////////////////////
    void printReportLine(ostream& out, const string& label, const string& value) {
        out << left << setw(25) << label << ": " << value << endl;
    }

	/////////////////////////// Full System Report  ///////////////////////

    void generateFullReport() {

        string totalUsers = getReportValue("SELECT COUNT(*) FROM USERS;");

        string totalCustomers = getReportValue("SELECT COUNT(*) FROM CUSTOMER;");

        string totalCars = getReportValue("SELECT COUNT(*) FROM CARS;");
        string availableCars = getReportValue("SELECT COUNT(*) FROM CARS WHERE status = 'available';");
        string rentedCars = getReportValue("SELECT COUNT(*) FROM CARS WHERE status = 'rented';");
        string inactiveCars = getReportValue("SELECT COUNT(*) FROM CARS WHERE status = 'inactive';");

        string totalRentals = getReportValue("SELECT COUNT(*) FROM RENTALS;");
        string activeRentals = getReportValue("SELECT COUNT(*) FROM RENTALS WHERE rental_status = 'active';");
        string completedRentals = getReportValue("SELECT COUNT(*) FROM RENTALS WHERE rental_status = 'completed';");
        string cancelledRentals = getReportValue("SELECT COUNT(*) FROM RENTALS WHERE rental_status = 'cancelled';");

        string totalRevenue = getReportValue(
            "SELECT COALESCE(SUM(I.amount), 0) "
            "FROM INVOICES I "
            "JOIN RENTALS R ON I.rental_id = R.rental_id "
            "WHERE R.rental_status != 'cancelled';"
        );

        string totalLateFine = getReportValue(
            "SELECT COALESCE(SUM(late_fine), 0) "
            "FROM RENTALS "
            "WHERE rental_status != 'cancelled';"
        );

        string mostRentedCar = getReportValue(
            "SELECT CONCAT(C.brand, ' ', C.model, ' (', COUNT(R.rental_id), ' rentals)') "
            "FROM RENTALS R "
            "JOIN CARS C ON R.car_id = C.car_id "
            "WHERE R.rental_status != 'cancelled' "
            "GROUP BY C.car_id, C.brand, C.model "
            "ORDER BY COUNT(R.rental_id) DESC "
            "LIMIT 1;"
        );

        ofstream reportFile("full_system_report.txt");

        if (!reportFile) {
            cout << "Error creating report file.\n";
            return;
        }

        auto printFullReport = [&](ostream& out) {

            cout << "\n================================================\n";
            cout << "              FULL SYSTEM REPORT\n";
            cout << "================================================\n\n";

            cout << "USERS\n";
            cout << "------------------------------------------------\n";
            printReportLine(cout, "Total Users", totalUsers);

            cout << "\nCUSTOMERS\n";
            cout << "------------------------------------------------\n";
            printReportLine(cout, "Total Customers", totalCustomers);

            cout << "\nCARS\n";
            cout << "------------------------------------------------\n";
            printReportLine(cout, "Total Cars", totalCars);
            printReportLine(cout, "Available Cars", availableCars);
            printReportLine(cout, "Rented Cars", rentedCars);
            printReportLine(cout, "Inactive Cars", inactiveCars);

            cout << "\nRENTALS\n";
            cout << "------------------------------------------------\n";
            printReportLine(cout, "Total Rentals", totalRentals);
            printReportLine(cout, "Active Rentals", activeRentals);
            printReportLine(cout, "Completed Rentals", completedRentals);
            printReportLine(cout, "Cancelled Rentals", cancelledRentals);

            cout << "\nFINANCIAL SUMMARY\n";
            cout << "------------------------------------------------\n";
            printReportLine(cout, "Total Revenue", "RM " + totalRevenue);
            printReportLine(cout, "Total Late Fine", "RM " + totalLateFine);

            cout << "\nMOST RENTED CAR\n";
            cout << "------------------------------------------------\n";
            printReportLine(cout, "Most Rented Car", mostRentedCar);

            out << "\n================================================\n";
            };

        printFullReport(cout);
        printFullReport(reportFile);

        reportFile.close();

        cout << "\nReport exported successfully to: full_system_report.txt\n";
    }


	//////////////////////////  Monthly Revenue Report  ///////////////////////

   void monthlyReport() {
        int month, year;

        cout << "\n========== MONTHLY REVENUE REPORT ==========\n";

        cout << "Enter Month (1-12): ";
        cin >> month;

        cout << "Enter Year: ";
        cin >> year;

        if (month < 1 || month > 12 || year <= 0) {
            cout << "Invalid month or year.\n";
            return;
        }

        string query =
            "SELECT SUM(INVOICES.amount) AS monthly_revenue "
            "FROM INVOICES "
            "JOIN RENTALS ON INVOICES.rental_id = RENTALS.rental_id "
            "WHERE MONTH(INVOICES.payment_date) = " + to_string(month) +
            " AND YEAR(INVOICES.payment_date) = " + to_string(year) +
            " AND RENTALS.rental_status != 'cancelled';";

        string revenue = executeQuery(query);

        cout << "\n====================================\n";
        cout << "       MONTHLY REVENUE REPORT\n";
        cout << "====================================\n";

        if (revenue.empty()) {
            cout << "No revenue data found.\n";
        }
        else {
            cout << "Month : " << month << endl;
            cout << "Year  : " << year << endl;
            cout << "Total Revenue : RM " << revenue << endl;
        }
	}

    ////////////////////////// Yearly Revenue Report  ///////////////////////

    void yearlyReport() {
        int year;
        cout << "\n========== YEARLY REVENUE REPORT ==========\n";
        cout << "Enter Year: ";
        cin >> year;
        if (year <= 0) {
            cout << "Invalid year.\n";
            return;
        }
        string query =
            "SELECT SUM(INVOICES.amount) AS yearly_revenue "
            "FROM INVOICES "
            "JOIN RENTALS ON INVOICES.rental_id = RENTALS.rental_id "
            "WHERE YEAR(INVOICES.payment_date) = " + to_string(year) +
            " AND RENTALS.rental_status != 'cancelled';";
        string revenue = executeQuery(query);
        cout << "\n====================================\n";
        cout << "       YEARLY REVENUE REPORT\n";
        cout << "====================================\n";
        if (revenue.empty()) {
            cout << "No revenue data found.\n";
        }
        else {
            cout << "Year  : " << year << endl;
            cout << "Total Revenue : RM " << revenue << endl;
        }
	}

	//////////////////////////// Total Rentals Report  ///////////////////////

    void totalRentalsReport() {

        string query =
            "SELECT COUNT(*) AS total_rentals "
            "FROM RENTALS "
            "WHERE rental_status != 'cancelled';";
        string totalRentals = executeQuery(query);
        cout << "\n====================================\n";
        cout << "       TOTAL RENTALS REPORT\n";
        cout << "====================================\n";
        if (totalRentals.empty()) {
            cout << "No rental data found.\n";
        }
        else {
            cout << "Total Rentals: " << totalRentals << endl;
        }
	}

	///////////////////// Most Rented Car Report  ///////////////////////

    void mostRentedCarReport() {
        cout << "\n========== MOST RENTED CAR REPORT ==========\n";

        string query =
            "SELECT CARS.car_id, CARS.brand, CARS.model, COUNT(RENTALS.rental_id) AS total_rented "
            "FROM RENTALS "
            "JOIN CARS ON RENTALS.car_id = CARS.car_id "
            "WHERE RENTALS.rental_status != 'cancelled' "
            "GROUP BY CARS.car_id, CARS.brand, CARS.model "
            "ORDER BY total_rented DESC "
            "LIMIT 1;";

        displayQuery(query);
    }



	//////////////////////////// view invoice  ///////////////////////

    void viewInvoice() {
        int rentalID;

        cout << "\n========== VIEW INVOICE ==========\n";
        cout << "Enter Rental ID: ";
        cin >> rentalID;

        printInvoice(rentalID);
    }

	///////////////////////////// print invoice  ///////////////////////

    void printInvoice(int rentalID) {
        try {
            sql::Statement* stmt = globalCon->createStatement();

            string query =
                "SELECT I.invoice_id, I.payment_date, I.payment_method, I.amount, "
                "R.rental_id, R.rental_date, R.return_date, R.days, R.total_price, R.late_fine,"
                "CU.customer_id, CU.full_name, "
                "C.brand, C.model, C.plate_number "
                "FROM INVOICES I "
                "JOIN RENTALS R ON I.rental_id = R.rental_id "
                "JOIN CUSTOMER CU ON R.customer_id = CU.customer_id "
                "JOIN CARS C ON R.car_id = C.car_id "
                "WHERE R.rental_id = " + to_string(rentalID) + ";";

            sql::ResultSet* res = stmt->executeQuery(query);

            if (res->next()) {
                cout << "\n========================================\n";
                cout << "              RENTAL INVOICE\n";
                cout << "========================================\n";
                cout << left << setw(18) << "Invoice ID" << ": " << res->getString("invoice_id") << endl;
                cout << left << setw(18) << "Rental ID" << ": " << res->getString("rental_id") << endl;
                cout << left << setw(18) << "Payment Date" << ": " << res->getString("payment_date") << endl;
                cout << left << setw(18) << "Payment Method" << ": " << res->getString("payment_method") << endl;
                cout << "----------------------------------------\n";
                cout << left << setw(18) << "Customer ID" << ": " << res->getString("customer_id") << endl;
                cout << left << setw(18) << "Customer Name" << ": " << res->getString("full_name") << endl;
                cout << "----------------------------------------\n";
                cout << left << setw(18) << "Car" << ": " << res->getString("brand") << " " << res->getString("model") << endl;
                cout << left << setw(18) << "Plate Number" << ": " << res->getString("plate_number") << endl;
                cout << left << setw(18) << "Rental Date" << ": " << res->getString("rental_date") << endl;
                cout << left << setw(18) << "Return Date" << ": " << res->getString("return_date") << endl;
                cout << left << setw(18) << "Days" << ": " << res->getString("days") << endl;
                cout << "----------------------------------------\n";
				cout << left << setw(18) << "Rental Price" << ": RM " << res->getString("total_price") << endl;
				cout << left << setw(18) << "Late Fine" << ": RM " << res->getString("late_fine") << endl;
                cout << "----------------------------------------\n";
                cout << left << setw(18) << "Total Amount" << ": RM " << res->getString("amount") << endl;
                cout << "========================================\n";
            }
            else {
                cout << "Invoice not found.\n";
            }

            delete res;
            delete stmt;
        }
        catch (sql::SQLException& e) {
            cout << "SQL Error: " << e.what() << endl;
        }
    }

	///////////////////////////// view my invoices ///////////////////////

    void viewMyInvoices() {
    cout << "\n============ MY INVOICES ============\n";

    try {
        sql::Statement* stmt = globalCon->createStatement();

        string query =
            "SELECT I.invoice_id, I.payment_date, I.payment_method, I.amount, "
            "R.rental_id, R.rental_date, R.return_date, R.days, R.total_price, R.late_fine, "
            "CU.customer_id, CU.full_name, "
            "C.brand, C.model, C.plate_number "
            "FROM INVOICES I "
            "JOIN RENTALS R ON I.rental_id = R.rental_id "
            "JOIN CUSTOMER CU ON R.customer_id = CU.customer_id "
            "JOIN CARS C ON R.car_id = C.car_id "
            "WHERE R.customer_id = '" + currentCustomerID + "';";

        sql::ResultSet* res = stmt->executeQuery(query);

        bool hasData = false;

        while (res->next()) {
            hasData = true;

            cout << "\n================================================\n";
            cout << "                RENTAL INVOICE\n";
            cout << "================================================\n";
            cout << left << setw(22) << "Invoice ID"     << ": " << res->getString("invoice_id") << endl;
            cout << left << setw(22) << "Rental ID"      << ": " << res->getString("rental_id") << endl;
            cout << left << setw(22) << "Payment Date"   << ": " << res->getString("payment_date") << endl;
            cout << left << setw(22) << "Payment Method" << ": " << res->getString("payment_method") << endl;

            cout << "------------------------------------------------\n";
            cout << left << setw(22) << "Customer ID"    << ": " << res->getString("customer_id") << endl;
            cout << left << setw(22) << "Customer Name"  << ": " << res->getString("full_name") << endl;

            cout << "------------------------------------------------\n";
            cout << left << setw(22) << "Car"            << ": " << res->getString("brand") << " " << res->getString("model") << endl;
            cout << left << setw(22) << "Plate Number"   << ": " << res->getString("plate_number") << endl;
            cout << left << setw(22) << "Rental Date"    << ": " << res->getString("rental_date") << endl;
            cout << left << setw(22) << "Return Date"    << ": " << res->getString("return_date") << endl;
            cout << left << setw(22) << "Days"           << ": " << res->getString("days") << endl;
            cout << "------------------------------------------------\n";
            cout << left << setw(22) << "Total Price"    << ": RM " << res->getString("total_price") << endl;
            cout << left << setw(22) << "Late Fine"      << ": RM " << res->getString("late_fine") << endl;
            cout << "------------------------------------------------\n";
            cout << left << setw(22) << "Total Amount"   << ": RM " << res->getString("amount") << endl;
            cout << "================================================\n";
        }

        if (!hasData) {
            cout << "\nNo invoices found.\n";
        }

        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cout << "SQL Error: " << e.what() << endl;
    }
}