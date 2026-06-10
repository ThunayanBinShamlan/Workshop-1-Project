#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include<string>
#include "Database.h"

using namespace std;


string login();
void registerCustomer();

string hiddenPassword();
void clearScreen();
void pauseScreen();

//////////// Calculation functions ////////////
double calculateRentalPrice(double pricePerDay, int days);
double calculateLateFine(double pricePerDay, int lateDays);


//////////// car management functions////////////

void manageCars();
void addCar();
void viewCars();
void updateCar();
void deleteCar();
void viewAvailableCars();

///////////// customer management functions////////////

void manageCustomers();
void addCustomer();
void viewCustomers();
void updateCustomer();
void deleteCustomer();
string getCustomerName();
void myProfile();
void showCustomerRentalInfo();
void viewRentalHistory();

//////////// user management functions ////////////

void manageUsers();
void addUser();
void viewUsers();
void updateUser();
void deleteUser();

////////////// rental management functions ////////////

void manageRentals();
void rentCar();
void returnCar();
void viewRentals();
void updateRental();
void cancelRental();
void viewActiveRentals();

///////////////	report functions ////////////

void reportsMenu();
void generateFullReport();
string getReportValue(const string& query);
void printReportLine(ostream& out, const string& label, const string& value);
void monthlyReport();
void yearlyReport();
void totalRentalsReport();
void mostRentedCarReport();
void cancelledRentalsReport(); 

void viewInvoice();
void printInvoice(int rentalID);
void viewMyInvoices();
#endif

