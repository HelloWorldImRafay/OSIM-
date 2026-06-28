#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "User.h"
#include "MFAHandler.h"
#include "SecureMessenger.h"
#include "Audit.h"
using namespace std;

    string PasswordManager ::hashPassword(const string &password)
    {
    string hashed = "";
    for (int i = 0; i < password.length(); i++)
    {
        hashed += char((password[i] + 5) % 126);
    }

    return hashed;
    }

    bool PasswordManager ::verifyPassword(const string &inputPassword, const string &storedHashed)
    {

    return hashPassword(inputPassword) == storedHashed;
    }

    User ::User(string n, string p, int l)
    : username(n), hashedPassword(PasswordManager::hashPassword(p)), clearanceLevel(l) {}

    User ::~User()
    {
    cout << "Record for " << username << " destroyed" << endl;
    }

    void User ::setUsername(string n) { username = n; }
    void User ::sethashedPassword(string p) { hashedPassword = PasswordManager::hashPassword(p); }
    void User ::setClearanceLevel(int l) { clearanceLevel = l; }

    string User ::getUsername() const { return username; }
    string User ::gethashedPassword() const { return hashedPassword; }
    int User ::getClearanceLevel() const { return clearanceLevel; }

    void User ::displayUserDetails()
    {
    cout << "User Details:\nUsername: " << username
            << "\nPassword: " << hashedPassword
            << "\nClearance Level: " << clearanceLevel << endl;
    }
    void loadUserfromfile(User *user)
    {
    fstream o1("User.txt", ios::in);
    string name, pword;
    int n;
    while (o1 >> name >> pword >> n)
    {
        if (user->getUsername() == name)
        {
        }
    }
    }

    User *User::createUser(const string &uname, const string &hashedPass, int level)
    {
    if (level == 1)
        return new Junior(uname, hashedPass, level);
    if (level == 2)
        return new Manager(uname, hashedPass, level);
    if (level == 3)
        return new Director(uname, hashedPass, level);
    if (level == 4)
        return new Executive(uname, hashedPass, level);
    return nullptr;
    }

    User *User::loadUser(const string &username, const string &password)
    {
    ifstream file("User.txt");
    string uname, hashedPass;
    int level;

    while (file >> uname >> hashedPass >> level)
    {
        if (uname == username && PasswordManager::verifyPassword(password, hashedPass))
        {
            return createUser(uname, hashedPass, level);
        }
    }
    return nullptr;
    }

    bool User ::userExists(const string &username)
    {
    ifstream file("User.txt");
    string uname, hash;
    int level;

    while (file >> uname >> hash >> level)
    {
        if (uname == username)
            return true;
    }

    return false;
    }


    bool User::userExists(const string &username, int *clearanceLevel)
    {
    ifstream file("User.txt");
    string uname, hashedPass;
    int level;

    while (file >> uname >> hashedPass >> level)
    {
        if (uname == username)
        {
            if (clearanceLevel != nullptr)
            {
                *clearanceLevel = level;
            }
            file.close();
            return true;
        }
    }

    file.close();
    return false;
    }

    User *User::loginUser(const string &username, const string &password)
    {
    User *user = loadUser(username, password); // Assuming loadUser is a static function in User class
    if (!user)
    {
        cout << "Invalid username or password!" << endl;
    }
    return user;
    }


    Employee ::Employee(string n, string p, int l) : User(n, p, l) {}

    Junior ::Junior(string n, string p, int l) : Employee(n, p, l) {}
    void Junior ::viewInbox() { cout << "Junior can view INFO messages.\n"; }
    void Junior ::assignTask() { cout << "Junior cant assign any tasks.\n"; }

    Manager ::Manager(string n, string p, int l) : Employee(n, p, l) {}
    void Manager ::viewInbox() { cout << "Manager can view INFO, ALERT, and PRIVATE messages.\n"; }
    void Manager ::assignTask() { cout << "Manager can assign tasks to Junior or Manager.\n"; }

    Director ::Director(string n, string p, int l) : Employee(n, p, l) {}
    void Director ::viewInbox() { cout << "Director can view INFO, ALERT, and PRIVATE messages.\n"; }
    void Director ::assignTask() { cout << "Director can assign tasks to Junior, Manager, or Director.\n"; }

    Executive ::Executive(string n, string p, int l) : Employee(n, p, l) {}

    void Executive ::viewInbox()
    {
    cout << "Executive can view INFO, ALERT, and PRIVATE messages.\n";
    }

    void Executive ::assignTask()
    {
    cout << "Executive can assign tasks to anyone (all roles).\n";
    }

    void Executive ::promoteUser(Employee &emp, int newLevel)
    {
    emp.setClearanceLevel(newLevel);
    cout << emp.getUsername() << " has been promoted to level " << newLevel << endl;
    }

    string User::getRole() const { return role; } 


    bool User::canSend(string type) const
    {
    return false;
    }

    bool Employee::canSend(string type) const
    {
    return type == "INFO";
    }

    bool Manager::canSend(string type) const
    {
    return true;
    }

    bool Director::canSend(string type) const
    {
    return true;
    }

    bool Executive::canSend(string type) const
    {
    return true;
    }

    void logUserActivity(const string &action, const string &username)
    {
    logActivity(action , username);
    }

    void registerUser()
    {
    string username, password;
    cout << "Enter username: ";
    cin >> username;

    bool isPasswordValid = false;

    while (!isPasswordValid)
    {
        cout << "Enter password: ";
        cin >> password;

        if (password.length() < 8)
        {
            cout << "Password must be at least 8 characters long.\n";
            continue;
        }

        bool hasUppercase = false;
        bool hasLowercase = false;
        bool hasDigit = false;
        bool hasSpecial = false;

        for (int i = 0; i < password.length(); i++)
        {
            char c = password[i];
            if (isupper(c))
                hasUppercase = true;
            if (islower(c))
                hasLowercase = true;
            if (isdigit(c))
                hasDigit = true;
            if (ispunct(c))
                hasSpecial = true;
        }

        if (hasUppercase && hasLowercase && hasDigit && hasSpecial)
        {
            isPasswordValid = true;
        }
        else
        {
            if (!hasUppercase)
                cout << "Password must contain at least one uppercase letter.\n";
            if (!hasLowercase)
                cout << "Password must contain at least one lowercase letter.\n";
            if (!hasDigit)
                cout << "Password must contain at least one digit.\n";
            if (!hasSpecial)
                cout << "Password must contain at least one special character (e.g., !@#$%^&*()).\n";
        }
    }

    ifstream infile("User.txt");
    string uname, pass;
    int level;

    while (infile >> uname >> pass >> level)
    {
        if (uname == username)
        {
            cout << "Username already exists.\n";
            infile.close();
            logUserActivity("Registration failed", username);
            return;
        }
    }
    infile.close();

    ofstream file("pending_users.txt", ios::app);
    if (file.is_open())
    {
        file << username << " " << PasswordManager::hashPassword(password) << " " << 1 << endl;
        cout << "Registration request submitted. Awaiting Director approval.\n";
        file.close();
        logUserActivity("Registration submitted", username);
    }
    else
    {
        cout << "Error writing to pending_users.txt\n";
        logUserActivity("Error writing registration data", username);
    }
    }

    void processPendingRegistrations(const User &currentUser)
    {
    if (currentUser.getClearanceLevel() < 3)
    {
        cout << "Access denied: Only Directors and Executives can approve users.\n";
        logActivity("Unauthorized registration approval attempt by " , currentUser.getUsername());
        return;
    }

    ifstream infile("pending_users.txt");
    ofstream approved("User.txt", ios::app);

    string uname, hashedPass;
    int level;
    bool anyPending = false;

    while (infile >> uname >> hashedPass >> level)
    {
        anyPending = true;
        char choice;
        cout << "Approve user '" << uname << "' (y = approve / n = reject): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y')
        {
            approved << uname << " " << hashedPass << " " << level << endl;
            cout << "User approved.\n";
            logActivity("User '" + uname + "' approved by " , currentUser.getUsername());
        }
        else
        {
            cout << "User rejected.\n";
            logActivity("User '" + uname + "' rejected by " , currentUser.getUsername());
        }
    }

    infile.close();
    approved.close();

    if (!anyPending)
    {
        cout << "No pending registrations.\n";
        return;
    }

    remove("pending_users.txt");
    }

    void User:: promoteUser(User* currentUser)
    {
    if (currentUser->getClearanceLevel() != 4) {  
        cout << "You do not have permission to promote users.\n";
        return;
    }

    string targetUsername;
    cout << "Enter username to promote: ";
    cin >> targetUsername;

    ifstream inFile("User.txt");
    ofstream tempFile("temp.txt");

    string uname, pass;
    int level;
    bool found = false;

    while (inFile >> uname >> pass >> level)
    {
        if (uname == targetUsername)
        {
            found = true;
            if (level < 3) 
            {
                level++;
                cout << "Promoted " << uname << " to level " << level << endl;
            }
            else
            {
                cout << uname << " is already at the maximum promotion level (Director).\n";
            }
            tempFile << uname << " " << pass << " " << level << endl;
        }
        else
        {
            tempFile << uname << " " << pass << " " << level << endl;
        }
    }

    inFile.close();
    tempFile.close();

    if (!found)
    {
        cout << "User not found.\n";
        remove("temp.txt");
    }
    else
    {
        remove("User.txt");
        rename("temp.txt", "User.txt");
    }
    }


    void searchAndUpdateUser()
    {
    cout << "Search User by:\n1. Username\n2. Level\nEnter choice (1 or 2): ";
    int choice;
    cin >> choice;

    string username;
    int level;
    bool found = false;
    ifstream inFile("User.txt");
    string uname, pass;
    int userLevel;

    ofstream tempFile("temp.txt");

    if (choice == 1)
    {
        cout << "Enter the username to search: ";
        cin >> username;

        // Search by username
        while (inFile >> uname >> pass >> userLevel)
        {
            if (uname == username)
            {
                found = true;
                cout << "User found: " << uname << " | Level: " << userLevel << endl;
                cout << "Do you want to update this user's info? (y/n): ";
                char updateChoice;
                cin >> updateChoice;

                if (updateChoice == 'y' || updateChoice == 'Y')
                {
                    cout << "Enter new username: ";
                    cin >> uname;
                    cout << "Enter new password: ";
                    cin >> pass;
                    pass = PasswordManager::hashPassword(pass);
                    tempFile << uname << " " << pass << " " << userLevel << endl;
                }
                else
                {
                    tempFile << uname << " " << pass << " " << userLevel << endl;
                }
            }
            else
            {
                tempFile << uname << " " << pass << " " << userLevel << endl;
            }
        }
    }
    else if (choice == 2)
    {
        cout << "Enter the level to search for: ";
        cin >> level;

        while (inFile >> uname >> pass >> userLevel)
        {
            if (userLevel == level)
            {
                found = true;
                cout << "User found: " << uname << " | Level: " << userLevel << endl;
                cout << "Do you want to update this user's info? (y/n): ";
                char updateChoice;
                cin >> updateChoice;

                if (updateChoice == 'y' || updateChoice == 'Y')
                {
                    cout << "Enter new username: ";
                    cin >> uname;
                    cout << "Enter new password: ";
                    cin >> pass;
                    pass = PasswordManager::hashPassword(pass);
                    tempFile << uname << " " << pass << " " << userLevel << endl;
                }
                else
                {
                    tempFile << uname << " " << pass << " " << userLevel << endl;
                }
            }
            else
            {
                tempFile << uname << " " << pass << " " << userLevel << endl;
            }
        }
    }
    else
    {
        cout << "Invalid choice.\n";
    }

    if (found)
    {
        remove("User.txt");
        rename("temp.txt", "User.txt");
        cout << "User information updated successfully.\n";
    }
    else
    {
        cout << "User not found.\n";
    }

    inFile.close();
    tempFile.close();
    }

    void loginUser()
    {
    const int MAX_PASS_ATTEMPTS = 3;
    int passAttempts = 0;

    while (passAttempts < MAX_PASS_ATTEMPTS)
    {

        string username, password;
        cout << "\nLogin (" << MAX_PASS_ATTEMPTS - passAttempts << " left)\n"
                << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        ifstream file("User.txt");
        if (!file)
        {
            cerr << "User.txt not found.\n";
            return;
        }

        string fUser, fHash;
        int fLvl;
        bool credentialsOK = false;
        while (file >> fUser >> fHash >> fLvl)
        {
            if (fUser == username && PasswordManager::verifyPassword(password, fHash))
            {
                credentialsOK = true;
                break;
            }
        }
        file.close();

        if (!credentialsOK)
        {
            ++passAttempts;
            cout << "Invalid username or password.\n";
            logUserActivity("Failed Authentication", username); // Log failed login
            continue;
        }


        User *tempUser = User::loadUser(username, password);
        if (!tempUser)
        {
            cout << "User authentication failed.\n";
            return;
        }
        MFAHandler mfa;
        if (!mfa.verifyOTP(tempUser))
        {
            ++passAttempts;
            cout << "2-factor authentication failed.\n";
            logUserActivity("Failed Authentication", username);
            continue;
        }

        User *loggedIn = nullptr;
        switch (fLvl)
        {
        case 4:
            loggedIn = new Executive(username, password, fLvl);
            break;
        case 3:
            loggedIn = new Director(username, password, fLvl);
            break;
        case 2:
            loggedIn = new Manager(username, password, fLvl);
            break;
        default:
            loggedIn = new Junior(username, password, fLvl);
            break;
        }

        cout << "Login successful!\n";
        logUserActivity("Login successful", username); 
        loggedIn->viewInbox();
        loggedIn->assignTask();
        delete loggedIn;
        return; 
    }

    cout << "Maximum login attempts exceeded – access denied.\n";
    logUserActivity("Failed Authentication", "N/A"); 
    }

    void changePassword(const string &username)
    {
    string currentPassword;
    cout << "Enter your current password: ";
    cin >> currentPassword;

    ifstream inFile("User.txt");
    string uname, pass;
    int level;
    bool passwordCorrect = false;

    while (inFile >> uname >> pass >> level)
    {
        if (uname == username && PasswordManager::verifyPassword(currentPassword, pass))
        {
            passwordCorrect = true;
            break;
        }
    }
    inFile.close();

    if (passwordCorrect)
    {
        string newPassword;
        bool isNewPasswordValid = false;

        while (!isNewPasswordValid)
        {
            cout << "Enter a new password: ";
            cin >> newPassword;

            if (newPassword.length() < 8)
            {
                cout << "Password must be at least 8 characters long.\n";
                continue;
            }

            bool hasUppercase = false;
            bool hasLowercase = false;
            bool hasDigit = false;
            bool hasSpecial = false;

            for (int i = 0; i < newPassword.length(); i++)
            {
                char c = newPassword[i];
                if (isupper(c))
                    hasUppercase = true;
                if (islower(c))
                    hasLowercase = true;
                if (isdigit(c))
                    hasDigit = true;
                if (ispunct(c))
                    hasSpecial = true;
            }

            if (hasUppercase && hasLowercase && hasDigit && hasSpecial)
            {
                isNewPasswordValid = true;
            }
            else
            {
                if (!hasUppercase)
                    cout << "Password must contain at least one uppercase letter.\n";
                if (!hasLowercase)
                    cout << "Password must contain at least one lowercase letter.\n";
                if (!hasDigit)
                    cout << "Password must contain at least one digit.\n";
                if (!hasSpecial)
                    cout << "Password must contain at least one special character (e.g., !@#$%^&*()).\n";
            }
        }

        ifstream fileIn("User.txt");
        ofstream fileOut("temp.txt");
        bool userFound = false;

        while (fileIn >> uname >> pass >> level)
        {
            if (uname == username)
            {

                fileOut << uname << " " << PasswordManager::hashPassword(newPassword) << " " << level << endl;
                userFound = true;
            }
            else
            {
                fileOut << uname << " " << pass << " " << level << endl;
            }
        }

        fileIn.close();
        fileOut.close();

        if (userFound)
        {
            remove("User.txt");
            rename("temp.txt", "User.txt");
            cout << "Your password has been updated successfully.\n";
        }
        else
        {
            remove("temp.txt");
            cout << "User not found.\n";
        }
    }
    else
    {
        cout << "Current password is incorrect.\n";
    }
    }



