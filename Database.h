#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <map>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

using namespace std;

extern sql::Connection* globalCon;
extern int currentUserID;
extern int currentCustomerID;

void connectToDatabase();
void closeDatabaseConnection();
void executeInstruction(const string& instruction);
string executeQuery(const string& query);
void displayQuery(const string& query);


#endif // DATABASE_H
