#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <conio.h>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

// functions declaration....
void menu();
void sleep(float);
void showExitMessage();
void showDefaultMessage();
void showErrorMessage(string);
void showSuccessMessage(string);
// hold screen until user presses a key
void holdScreen();
// display a text as a specially formatted header
void buildHeader(string);
// function to display message before exiting the app...
void close(); //
double inputDouble();

/* Class to handle login and password reset feature */
class Authenticate
{
private:
    string username;
    string password;

public:
    void login()
    {
        system("CLS");
        string username;
        string password;
        cout << "============================================" << endl;
        cout << "\t\tLogin" << endl;
        cout << "============================================" << endl;
        cout << "Enter username: ";
        getline(cin, username);
        cout << "Enter password: ";
        getline(cin, password);
        // cout << endl;
        if (verify(username, password))
        {
            system("CLS");
            cout << "============================================" << endl;
            cout << "\tAuthentication Successfull...." << endl;
            sleep(0.5);
            cout << "\tEntering the Shop......." << endl;
            sleep(0.2);
            cout << "============================================" << endl;
            sleep(1);
            system("CLS");
            menu();
        }
        else
        {
            system("CLS");
            cout << "============================================" << endl;
            cout << "\tAuthentication Failed..." << endl;
            cout << "============================================" << endl;
            sleep(1.5);
            login();
        }
    }
    bool validate(string u, string p)
    {
        if (u.length() < 5)
        {
            // cout << "Username is too short...";
            return false;
        }
        else if (p.length() < 8)
        {
            // cout << "Password must be atleast 8 characters long...";
            return false;
        }
        cout << "Valid" << endl;
        return true;
    }
    void save(string username, string password)
    {
        try
        {
            ofstream file;
            file.open("credentials.txt");
            if (!file)
            {
                throw runtime_error("Couldn't create a file....");
            }
            file << username << endl;
            file << password << endl;
            system("CLS");
            cout << "============================================" << endl;
            cout << "\tPassword Reset Succesfull..." << endl;
            cout << "============================================" << endl;
            file.close();
            sleep(1.5);
        }
        catch (exception &e)
        {
            cerr << e.what() << endl;
        }
    }

    void resetPassword()
    {

        system("CLS");
        string user;
        string pass;
        fflush(stdin);
        cout << "============================================" << endl;
        cout << "\tReset Password" << endl;
        cout << "============================================" << endl;
        cout << "\tNOTE:" << endl;
        cout << "\tUsername must have atleast 5 charcters" << endl;
        cout << "\tPassword must have atleast 8 characters" << endl;
        cout << "============================================" << endl;

        // cin.ignore();
        cout << "Enter username: ";
        cin >> user;
        cout << "Enter password: ";
        cin >> pass;
        if (validate(user, pass))
        {
            save(user, pass);
        }
        else
        {
            system("CLS");
            cout << "============================================" << endl;
            cout << "\nFailed....." << endl;
            cout << "============================================" << endl;
            sleep(1.5);
        }
    }
    bool verify(string u, string p)
    {
        try
        {
            ifstream file;
            file.open("credentials.txt");
            string user, pass;
            if (file)
            {
                getline(file, user);
                getline(file, pass);
            }
            if (!file)
            {
                user = "admin";
                pass = "admin";
            }
            // cout << "User => " << user << "Pass => " << pass << endl;
            if (u == user && p == pass)
            {
                return true;
            }
            return false;
        }

        catch (exception &e)
        {
            cerr << e.what() << endl;
        }
    }
};

/* Class to handle product related stuffs */
class Product
{
private:
    int id;
    string name;
    int quantity;
    string unit;
    double costPrice;
    double sellingPrice;

public:
    void ledger()
    {
        double totalCp = 0;
        double totalSp = 0;
        double result;
        system("CLS");
        buildHeader("Profit/Loss Ledger");
        try
        {
            ifstream file;
            file.open("products.dat", ios::in | ios::binary);
            string read;

            if (!file)
            {
                throw runtime_error("Coudn't open/locate the file...");
            }

            file.read((char *)this, sizeof(*this));
            while (!file.eof())
            {
                totalCp += (costPrice * quantity);
                totalSp += (sellingPrice * quantity);
                file.read((char *)this, sizeof(*this));
            }
            file.close();

            cout << "Totoal Amount oF Purchase:" << endl;
            cout << "Rs." << totalCp << endl;
            cout << "Total Amount Received After Selling Products:" << endl;
            cout << "Rs." << totalSp << endl;
            result = totalSp - totalCp;
            cout << "---------------------------------------------------------------" << endl;
            if (result >= 0)
            {
                cout << "Total Profit => Rs." << result << endl;
            }
            else
            {
                cout << "Total Loss => Rs." << -result << endl;
            }
        }

        catch (exception &e)
        {
            showErrorMessage(e.what());
            // sleep(2);
        }
    }

    void update()
    {
        system("CLS");
        buildHeader("Update Product");
        cout << "Enter the product ID: ";
        int productID;
        productID = inputDouble();
        try
        {
            bool isFound = false;
            fstream file; // we want to both read and write,so...
            file.open("products.dat", ios::in | ios::out | ios::ate | ios::binary);
            string read;
            if (!file)
            {
                throw runtime_error("Coudn't open/locate the file...");
            }
            file.seekg(0);
            file.read((char *)this, sizeof(*this));
            while (!file.eof())
            {
                if (productID == id)
                {
                    isFound = true;
                    system("CLS");
                    buildHeader("Provide the updated data");
                    getProductData();
                    long pos = file.tellp();
                    file.seekp(pos - sizeof(*this));
                    file.write((char *)this, sizeof(*this));
                    system("CLS");
                    showSuccessMessage("Product successfully updated...");
                    break;
                }
                file.read((char *)this, sizeof(*this));
            }
            file.close();

            if (!isFound)
            {
                showErrorMessage("No product is available with provided id...");
            }
        }
        catch (exception &e)
        {
            showErrorMessage(e.what());
            // sleep(3);
        }
    }

    void search()
    {
        system("CLS");
        buildHeader("Search Product");
        int productId;
        cout << "Enter a product id to search: ";
        productId = inputDouble();

        system("CLS");
        buildHeader("Search Result");
        try
        {
            ifstream file;
            file.open("products.dat", ios::in | ios::binary);
            string read;
            if (!file)
            {
                throw runtime_error("Coudn't open/locate the file...");
            }
            bool isFound = false;

            file.read((char *)this, sizeof(*this));
            while (!file.eof())
            {
                if (productId == id)
                {
                    isFound = true;
                    cout << setw(10) << left << "ID" << setw(25) << left << "Product Name" << setw(10) << left << "Quantity" << setw(15) << left << "Unit" << setw(15) << left << "Cost Price" << setw(15) << left << "Selling Price" << endl;
                    cout << setw(10) << left << id << setw(25) << left << name << setw(10) << left << quantity << setw(15) << left << unit << setw(15) << left << costPrice << setw(15) << left << sellingPrice << endl;
                    break;
                }
                file.read((char *)this, sizeof(*this));
            }
            if (!isFound)
            {
                showErrorMessage("No product is available with provided id...");
            }
        }
        catch (exception &e)
        {
            showErrorMessage(e.what());
            // sleep(2);
        }
    }

    void viewAllProducts()
    {
        system("CLS");
        buildHeader("Shop Inventory");
        int totalProducts = 0;
        try
        {
            ifstream file;
            file.open("products.dat", ios::in | ios::binary);
            string read;

            if (!file)
            {
                throw runtime_error("Coudn't open/locate the file...");
            }

            file.read((char *)this, sizeof(*this));
            cout << setw(10) << left << "ID" << setw(25) << left << "Product Name" << setw(10) << left << "Quantity" << setw(15) << left << "Unit" << setw(15) << left << "Cost Price" << setw(15) << left << "Selling Price" << endl;
            while (!file.eof())
            {
                ++totalProducts;
                cout << setw(10) << left << id << setw(25) << left << name << setw(10) << left << quantity << setw(15) << left << unit << setw(15) << left << costPrice << setw(15) << left << sellingPrice << endl;

                file.read((char *)this, sizeof(*this));
            }
            file.close();
            cout << endl;
            cout << "-----------------------------------------------------------------" << endl;
            cout << "Total Products => " << totalProducts << endl;
        }

        catch (exception &e)
        {
            showErrorMessage(e.what());
            // sleep(3);
        }
    }

    void getProductData()
    {
        cout << "Enter the product ID: ";
        this->id = int(inputDouble());
        fflush(stdin);
        cout << "Enter product name: ";
        getline(cin, this->name);
        cout << "Enter the quantity purchased: ";
        this->quantity = int(inputDouble());
        fflush(stdin);
        cout << "Enter the unit of product(kg/dozen): ";
        getline(cin, this->unit);
        cout << "Enter the cost price per unit: ";
        this->costPrice = inputDouble();
        cout << "Enter the selling price per unit: ";
        this->sellingPrice = inputDouble();
    }

    void registerNewProduct()
    {
        fflush(stdin);
        system("CLS");
        cout << "==============================================================================" << endl;
        cout << "\t\tRegister Product" << endl;
        cout << "==============================================================================" << endl;

        // get the product details
        getProductData();

        ofstream file;
        if (!file)
        {
            throw runtime_error("Couldn't open/create file...");
        }
        try
        {
            file.open("products.dat", ios::app | ios::binary);
            file.write((char *)this, sizeof(*this));
            showSuccessMessage("Product is registered successfully..");
            sleep(1.5);
            file.close();
        }
        catch (exception &e)
        {
            cerr << e.what() << endl;
            sleep(2);
            return;
        }
    }
};

/* Class to hadle the employee data */
class Employee
{
private:
    int id;
    string fullName;
    string address;
    double salary;
    string contact;

public:
    void getEmployeeData()
    {
        cout << "Enter employee ID: ";
        id = inputDouble();
        fflush(stdin);
        cout << "Enter full name: ";
        getline(cin, fullName);
        cout << "Enter contact number: ";
        getline(cin, contact);
        cout << "Enter address: ";
        getline(cin, address);
        cout << "Enter monthly salary: ";
        salary = inputDouble();
    }

    void recruit()
    {
        system("CLS");
        buildHeader("Recruit New Employee");
        getEmployeeData();
        // let's write it to the file now
        ofstream file;
        if (!file)
        {
            throw runtime_error("Couldn't open/create file...");
        }
        try
        {
            file.open("employees.dat", ios::app | ios::binary);
            file.write((char *)this, sizeof(*this));
            showSuccessMessage("Employee is registered successfully..");
            sleep(1.5);
            file.close();
        }
        catch (exception &e)
        {
            cerr << e.what() << endl;
            sleep(2);
            return;
        }
    }

    void showAllEmployeesDetail()
    {
        system("CLS");
        buildHeader("Employees Record");
        int totalEmployees = 0;
        try
        {
            ifstream file;
            file.open("employees.dat", ios::in | ios::binary);
            string read;

            if (!file)
            {
                throw runtime_error("Coudn't open/locate the file...");
            }

            file.read((char *)this, sizeof(*this));
            cout << setw(10) << left << "ID" << setw(25) << left << "Name" << setw(15) << left << "Contact" << setw(15) << left << "Salary" << setw(20) << left << "Address" << endl;
            while (!file.eof())
            {
                ++totalEmployees;
                cout << setw(10) << left << id << setw(25) << left << fullName << setw(15) << left << contact << setw(15) << left << salary << setw(20) << left << address << endl;
                file.read((char *)this, sizeof(*this));
            }
            // file.close();
            cout << endl;
            cout << "--------------------------------------------------------------" << endl;
            cout << "Total Employees => " << totalEmployees << endl;
        }
        catch (exception &e)
        {
            showErrorMessage(e.what());
            // sleep(3);
        }
    }

    void search()
    {
        system("CLS");
        buildHeader("Search Employee");
        int employee;
        cout << "Enter the employee id to search: ";
        employee = inputDouble();

        system("CLS");
        buildHeader("Search Result");
        try
        {
            ifstream file;
            file.open("employees.dat", ios::in | ios::binary);
            string read;
            if (!file)
            {
                throw runtime_error("Coudn't open/locate the file...");
            }
            bool isFound = false;

            file.read((char *)this, sizeof(*this));
            while (!file.eof())
            {
                if (employee == id)
                {
                    isFound = true;
                    cout << setw(10) << left << "ID" << setw(25) << left << "Name" << setw(10) << left << "Contact" << setw(15) << left << "Salary" << setw(15) << left << "Address" << endl;
                    cout << setw(10) << left << id << setw(25) << left << fullName << setw(10) << left << contact << setw(15) << left << salary << setw(15) << left << address << endl;
                    break;
                }
                file.read((char *)this, sizeof(*this));
            }
            if (!isFound)
            {
                showErrorMessage("No employee of provided id is available...");
            }
        }
        catch (exception &e)
        {
            showErrorMessage(e.what());
            // sleep(2);
        }
    }

    void update()
    {
        system("CLS");
        buildHeader("Update Employee Details");
        cout << "Enter the employee ID: ";
        int employeeId;
        employeeId = inputDouble();
        try
        {
            bool isFound = false;
            fstream file; // we want to both read and write,so...
            file.open("employees.dat", ios::in | ios::out | ios::ate | ios::binary);
            string read;
            if (!file)
            {
                throw runtime_error("Coudn't open/locate the file...");
            }
            file.seekg(0);
            file.read((char *)this, sizeof(*this));
            while (!file.eof())
            {
                if (employeeId == id)
                {
                    isFound = true;
                    system("CLS");
                    buildHeader("Update Employee Details");
                    getEmployeeData();
                    long int pos = file.tellp();
                    file.seekp(pos - sizeof(*this));
                    file.write((char *)this, sizeof(*this));
                    system("CLS");
                    showSuccessMessage("Product successfully updated...");
                    break;
                }
                file.read((char *)this, sizeof(*this));
            }
            file.close();

            if (!isFound)
            {
                showErrorMessage("No employee is available with provided id...");
            }
        }
        catch (exception &e)
        {
            showErrorMessage(e.what());
            // sleep(3);
        }
    }
};

/* Driver function of our program */

int main()
{
    while (true)
    {
        fflush(stdin);
        system("CLS");
        string choice;
        cout << "==============================================================================" << endl;
        cout << "                                  WELCOME TO                                  " << endl;
        cout << "                            SHOP MANAGEMENT SYSTEM                            " << endl;
        cout << "==============================================================================" << endl;
        cout << "                           1. ENTER THE SHOP                                  " << endl;
        cout << "                           2. EXIT                                            " << endl;
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "Your choice: ";
        cin >> choice;
        if (!cin.good())
        {
            showDefaultMessage();
            sleep(2);
            continue;
        }
        if (choice == "1")
        {
            fflush(stdin);
            Authenticate a;
            a.login();
            // menu();
        }
        else if (choice == "2")
        {
            system("CLS");
            showExitMessage();
            exit(0);
        }
        else
        {
            system("CLS");
            showDefaultMessage();
            sleep(2.5);
            system("CLS");
            continue;
        }
    }
    return 0;
}

/// userdefined functions...

void sleep(float seconds)
{
    clock_t startClock = clock();
    float secondsAhead = seconds * CLOCKS_PER_SEC;
    // do nothing until the elapsed time has passed.
    while (clock() < startClock + secondsAhead)
        ;
    return;
}

void showDefaultMessage()
{
    cout << "============================================" << endl;
    cout << "\tInvalid input...." << endl;
    cout << "\tPlease enter a valid number...." << endl;
    cout << "============================================" << endl;
}

void showExitMessage()
{
    cout << "============================================" << endl;
    cout << "\tSuccessfully exited the shop" << endl;
    cout << "============================================" << endl;
}

void showErrorMessage(string message)
{
    system("CLS");
    cout << "==============================================================================" << endl;
    cout << "**Error**" << endl;
    cout << "------------------" << endl;
    cout << message << endl;
    cout << "==============================================================================" << endl;
}

void showSuccessMessage(string message)
{
    system("CLS");
    cout << "==============================================================================" << endl;
    cout << "\t\t\tSuccessfull" << endl;
    cout << "---------------------" << endl;
    cout << message << endl;
    cout << "==============================================================================" << endl;
}

void menu()
{
    while (true)
    {
        fflush(stdin);
        system("CLS");
        cout << "==============================================================================" << endl;
        cout << "                                   WELCOME TO SHOP                            " << endl;
        cout << "==============================================================================" << endl;
        cout << "                                     SHOP MENU                                " << endl;
        cout << "------------------------------------------------------------------------------" << endl;
        cout << "1. Add new product entry" << endl;
        cout << "2. Recruit new employee" << endl;
        cout << "3. All products overview" << endl;
        cout << "4. Modify product details" << endl;
        cout << "5. Search product details" << endl;
        cout << "6. All employees details" << endl;
        cout << "7. Modify employee details" << endl;
        cout << "8. Search emplyee details" << endl;
        cout << "9. Account Ledger Book" << endl;
        cout << "10. Reset Password" << endl;
        cout << "11. Exit" << endl;
        cout << "------------------------------------------------------------------------------" << endl;
        string choice;
        cout << "Your choice: ";
        cin >> choice;
        if (!cin.good())
        {
            showDefaultMessage();
            sleep(2);
            fflush(stdin);
            menu();
        }
        if (choice == "1")
        {
            fflush(stdin);
            Product p;
            p.registerNewProduct();
            holdScreen();
        }
        else if (choice == "2")
        {
            fflush(stdin);
            Employee e;
            e.recruit();
            holdScreen();
        }
        else if (choice == "3")
        {
            fflush(stdin);
            Product p;
            p.viewAllProducts();
            holdScreen();
        }
        else if (choice == "4")
        {
            Product p;
            p.update();
            holdScreen();
        }
        else if (choice == "5")
        {
            fflush(stdin);
            Product p;
            p.search();
            holdScreen();
        }
        else if (choice == "6")
        {
            fflush(stdin);
            Employee e;
            e.showAllEmployeesDetail();
            holdScreen();
        }
        else if (choice == "7")
        {
            fflush(stdin);
            Employee e;
            e.update();
            holdScreen();
        }
        else if (choice == "8")
        {
            fflush(stdin);
            Employee e;
            e.search();
            holdScreen();
        }
        else if (choice == "9")
        {
            fflush(stdin);
            Product p;
            p.ledger();
            holdScreen();
        }
        else if (choice == "10")
        {
            fflush(stdin);
            system("CLS");
            Authenticate a;
            a.resetPassword();
            holdScreen();
        }
        else if (choice == "11")
        {
            system("CLS");
            showExitMessage();
            exit(0);
        }
        else
        {
            system("CLS");
            showDefaultMessage();
            sleep(3);
            fflush(stdin);
            continue;
        }
    }
}

void holdScreen()
{
    cout << "------------------------------------------------------" << endl;
    cout << "Enter any key to continue...";
    getch();
}

void buildHeader(string title)
{
    cout << "============================================================" << endl;
    cout << "\t\t" << title << endl;
    cout << "============================================================" << endl;
}

double inputDouble()
{
    double num;
    cin >> num;
    if (!cin.good())
    {
        showErrorMessage("Please enter a number..");
        close();
    }
    return num;
}

void close()
{
    sleep(1.5);
    system("CLS");
    showExitMessage();
    sleep(2);
    exit(0);
}
