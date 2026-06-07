#include "Database.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <fstream>

sql::Connection* globalCon = nullptr;

int currentUserID = 0;
std::string currentCustomerID = "";

void connectToDatabase() {
    try {
        // Get a pointer to the database driver instance
        sql::Driver* driver = get_driver_instance();
        // Establish connection to the database server at the specified address and port
        globalCon = driver->connect("tcp://127.0.0.1:3306", "root", "");
        // Set the current database schema to "car rental"
        globalCon->setSchema("car rental");
    }
    catch (sql::SQLException& e) {
        // Output error details if an SQL exception occurs
        cout << "SQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode()
            << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

void closeDatabaseConnection() {
    // Check if the global connection object is not null
    if (globalCon) {

        globalCon->close(); // Close the database connection
        delete globalCon; // Delete the connection object to free memory
        globalCon = nullptr; // Reset the global connection pointer to null to avoid dangling pointer
    }
}

void executeInstruction(const string& instruction) {
    try {
        // Create a new SQL statement using the global connection
        sql::Statement* stmt = globalCon->createStatement();
        // Execute the SQL instruction passed to the function
        stmt->execute(instruction);
        // Delete the statement to free memory
        delete stmt;
    }
    catch (sql::SQLException& e) {
        // Output error details if an SQL exception occurs during the operation
        cout << "SQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode()
            << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

string executeQuery(const string& query) {
    string result = "";

    try {
        sql::Statement* stmt = globalCon->createStatement();
        sql::ResultSet* res = stmt->executeQuery(query);

        if (res->next()) {
            result = res->getString(1);
        }

        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cout << "SQL Error: " << e.what()
            << " (MySQL error code: " << e.getErrorCode()
            << ", SQLState: " << e.getSQLState() << " )" << endl;
    }

    return result;
}


void displayQuery(const string& query) {
    try {
        sql::Statement* stmt = globalCon->createStatement();
        sql::ResultSet* res = stmt->executeQuery(query);
        sql::ResultSetMetaData* meta = res->getMetaData();

        int columns = meta->getColumnCount();

        cout << "\n------------------------------------------------------------\n";

        // Print column names
        for (int i = 1; i <= columns; i++) {
            cout << meta->getColumnName(i) << "\t";
        }

        cout << "\n------------------------------------------------------------\n";

        // Print rows
        while (res->next()) {
            for (int i = 1; i <= columns; i++) {
                cout << res->getString(i) << "\t";
            }
            cout << endl;
        }

        cout << "------------------------------------------------------------\n";

        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cout << "SQL Error: " << e.what()
            << " (MySQL error code: " << e.getErrorCode()
            << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

