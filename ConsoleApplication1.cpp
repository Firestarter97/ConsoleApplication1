// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <random>
#include <ctime>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>

//In C++, forward declaration for a class is a way to declare the existence of a class without providing its full definition. 
//This is useful when you need to use a class in a context where only the class name is needed, like declaring a pointer or a 
//reference to an object of that class.
class person;

class chase
{
    //private members of class chase
    private:
        long int accountNum;
        long int balanceAmount;
        std::string userName;
        std::string password;

    //public members of chase class
    public:
        chase(long int accountNum, long int balanceAmount, std::string userName, std::string password)
        {
            this->accountNum = accountNum;
            this->balanceAmount = balanceAmount;
            this->userName = userName;
            this->password = password;
        }
        //default constructor for chase class
        chase() : accountNum{0}, balanceAmount{0} {}

        void displayCreateNewAcc(std::unordered_map<std::string, person>& userDatabase);
        void displaySignIn(std::unordered_map<std::string, person>& userDatabase); 
        void displayAccount(std::unordered_map<std::string, person>& userDatabase);
        void displayWelcomeScreen(std::unordered_map<std::string, person>& userDatabase);
        std::string hashPassword(const std::string& password);

        //setters to modify the private variables
        void setaccNum(long int accountNum)
        {
            this->accountNum = accountNum;
        }

        void setUserName(std::string userName)
        {
            this->userName = userName;
        }

        void setPassword(std::string password)
        {
            this->password = password;
        }

        // getters to access the private variables
        long int const getaccNum()
        {
            return accountNum;
        }

        std::string getUserName() const
        {
            return userName;
        }

        std::string getPassword()
        {
            return password;
        }
};

//access specifier ex) public, private, protected. When using the derived class object to access the fncs of the base class
//you must use an access specifier in order for you to access those fncs. The differences between using these specifiers is:
//(PUBLIC) All public and protected members of the base class become public members in the derived class. 
//(PROTECTED) All public and protected members of the base class become protected members in the derived class.
//(PRIVATE) All public and protected members of the base class become private members in the derived class.
class person: public chase
{
    private:
        std::string name;
    
    public:
        //if the base class has a constructor, the derived class constructor must provide initial values for the base constructor
        //afterwards you can initilize your own varible/variables
        person(long int accountNum, long int balanceAmount, std::string userName, std::string password, std::string name)
            : chase{accountNum, balanceAmount, userName, password}, name{name} {}

        void setname(std::string newName)
        {
            name = newName;
        }
        std::string getname()
        {
            return name;
        }
        //default constructor for chase class
        person() {}
};

std::string chase::hashPassword(const std::string& password)
{
    OpenSSL_add_all_algorithms();

    const EVP_MD* md = EVP_sha256();
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();

    if (EVP_DigestInit_ex(mdctx, md, NULL) != 1)
    {
        std::cerr << "Error initializing digest" << std::endl;
        return "";
    }

    if (EVP_DigestUpdate(mdctx, password.c_str(), password.length()) != 1)
    {
        std::cerr << "Error updating digest" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;

    if (EVP_DigestFinal_ex(mdctx, hash, &hashLen) != 1)
    {
        std::cerr << "Error finalizing digest" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    EVP_MD_CTX_free(mdctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < hashLen; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

void chase::displayWelcomeScreen(std::unordered_map<std::string, person>& userDatabase)
{
    int option = 0;
    do
    {
        std::system("cls");
        std::cout << "Welcome to Chase";
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::system("cls");

        std::cout << "Press 1 for sign in\nPress 2 for creating new account\n";
        std::cin >> option;

        switch (option)
        {
        case 1:
            std::system("cls");
            displaySignIn(userDatabase);
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        case 2:
            std::system("cls");
            displayCreateNewAcc(userDatabase);
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        default:
            std::system("cls");
            std::cout << "Invalid option. Please enter 1 sign in and Press 2 for creating new account.\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore any remaining characters, including Enter
            break;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (1);
}

void chase::displayCreateNewAcc(std::unordered_map<std::string, person>& userDatabase)
{
    std::string newName = "";
    std::string newUserName = "";
    std::string newPassword = "";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter your first and last name: ";
    std::getline(std::cin, newName);

    std::cout << "Create a new username: ";
    std::cin >> newUserName;

    //check if username already exists or not.
    while (userDatabase.find(newUserName) != userDatabase.end()) {
        std::cout << "Username is already taken. Please try again with a different username.\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::system("cls");
        std::cout << "Enter your first and last name: " << newUserName << std::endl;
        std::cout << "Create a new username: ";
        std::cin >> newUserName;
    }

    std::cout << "Create a new password: ";
    std::cin >> newPassword;

    newPassword = hashPassword(newPassword);

    // Seed the generators with different values
    std::mt19937 accountNumGenerator(static_cast<unsigned long int>(std::time(0))); // Seed for account numbers

    // Distributions for account numbers and IDs
    std::uniform_int_distribution<long int> accountNumDistribution(100000000, 999999999); // Range for a 9-digit number

    person newPerson(accountNumDistribution(accountNumGenerator), balanceAmount, newUserName, newPassword, newName);
    userDatabase[newUserName] = newPerson;
    std::system("cls");
    std::cout << "Your account has been created!\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    displayWelcomeScreen(userDatabase);
}

void chase::displaySignIn(std::unordered_map<std::string, person>& userDatabase)
{
    std::string tempUsername = "";
    std::string tempPassword = "";
    char tryAgain = '\0';

    do {
        std::system("cls");
        std::cout << "--------------SIGN IN PAGE------------------\n\n";
        std::cout << "Enter Username: ";
        std::cin >> tempUsername;

        // Check if the user exists in the map
        auto userIter = userDatabase.find(tempUsername);

        if (userIter != userDatabase.end())
        {
            std::cout << "\nEnter Password: ";
            std::cin >> tempPassword;

            tempPassword = userIter->second.hashPassword(tempPassword);

            if (userIter->second.getPassword() == tempPassword)
            {
                std::system("cls");
                userIter->second.displayAccount(userDatabase);
                return;
            }
        }

        std::system("cls");
        std::cout << "Incorrect Username or Password. Press 1 to try again or Press 2 to Exit\n";
        std::cin >> tryAgain;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear any extra input
        if (tryAgain == '2')
        {
            displayWelcomeScreen(userDatabase);
        }

    } while (tryAgain == '1');
}

void chase::displayAccount(std::unordered_map<std::string, person>& userDatabase)
{
    int option = 0;

    do {
        std::cout << "\nAccount Menu For User: " << userDatabase[getUserName()].getname() << "\t" << "Account Number: " << userDatabase[getUserName()].getaccNum() << std::endl;
        std::cout << "1. Check Balance\n";
        std::cout << "2. Deposit Money\n";
        std::cout << "3. Withdraw Money\n";
        std::cout << "4. Change Password\n";
        std::cout << "5. Sign Out\n";
        std::cin >> option;

        std::string newPassword;

        switch (option)
        {
        case 1:
            std::system("cls");
            std::cout << "Balance: $" << balanceAmount << std::endl;
            break;
        case 2:
            std::system("cls");
            long int depositAmount;
            std::cout << "Enter the amount to deposit: $";
            std::cin >> depositAmount;
            balanceAmount += depositAmount;
            std::system("cls");
            std::cout << "Deposit successful. New balance: $" << balanceAmount << std::endl;
            break;
        case 3:
            std::system("cls");
            long int withdrawAmount;
            std::cout << "Enter the amount to withdraw: $";
            std::cin >> withdrawAmount;
            std::system("cls");
            if (withdrawAmount <= balanceAmount) {
                balanceAmount -= withdrawAmount;
                std::cout << "Withdrawal successful. New balance: $" << balanceAmount << std::endl;
            }
            else {
                std::cout << "Insufficient balance for withdrawal.\n";
            }
            break;
        case 4:
            std::system("cls");
            std::cout << "Enter a new password: ";
            std::cin >> newPassword;
            this->password = newPassword;
            std::system("cls");
            std::cout << "Password changed successfully.\n";
            break;
        case 5:
            std::system("cls");
            std::cout << "Signing out...\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            displayWelcomeScreen(userDatabase);
            break;
        default:
            std::system("cls");
            std::cout << "Invalid option. Available options are: 1, 2, 3, 4, 5\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore any remaining characters, including Enter
            break;
        };
    } while (1);
}

int main()
{
    std::unordered_map<std::string, person> userDatabase;
    person obj;
    obj.displayWelcomeScreen(userDatabase);
}