#include <iostream>
#include "Database.h"
#include "Menus.h"
#include "Functions.h"

using namespace std;

int main() {

	connectToDatabase();

	showMainMenu();

	closeDatabaseConnection();	

	return 0;
}

