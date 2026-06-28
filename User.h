#ifndef USER_H
#define USER_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class PasswordManager {
public:
    static string hashPassword(const string& password);
    static bool verifyPassword(const string& inputPassword, const string& storedHashed);
};

class User {
protected:
    string username;
    string hashedPassword;
    int clearanceLevel;
    string role;

public:
    User(string n = "", string p = "", int l = 0);
    virtual ~User();

    virtual void viewInbox() = 0;
    virtual void assignTask() = 0;

    void setUsername(string n);
    void sethashedPassword(string p);
    void setClearanceLevel(int l);

    string getUsername() const;
    string getRole() const;
    string gethashedPassword() const;
    int getClearanceLevel() const;
    static bool userExists(const string& username);
    static User* createUser(const string& uname, const string& hashedPass, int level);
    static User* loadUser(const string& username, const string& password);
    static bool userExists(const string& username, int* clearanceLevel);
    virtual bool canSend(string type) const; 
    static User* loginUser(const string& username, const string& password);
    void displayUserDetails();
    void promoteUser(User* currentUser);  
};



class Employee : public User {
public:
    Employee(string n = "", string p = "", int l = 0);
    bool canSend(string type) const override;

};

class Junior : public Employee {
public:
    Junior(string n = "", string p = "", int l = 1);
    void viewInbox() override;
    void assignTask() override;
};

class Manager : public Employee {
public:
    Manager(string n = "", string p = "", int l = 2);
    void viewInbox() override;
    void assignTask() override;
    bool canSend(string type) const override;

};

class Director : public Employee {
public:
    Director(string n = "", string p = "", int l = 3);
    void viewInbox() override;
    void assignTask() override;
    bool canSend(string type) const override;

};

class Executive : public Employee {
public:
    Executive(string n = "", string p = "", int l = 4);
    void viewInbox() override;
    void assignTask() override;
    void promoteUser(Employee& emp, int newLevel);
    bool canSend(string type) const override;

};

void logUserActivity(const string& action, const string& username);
void registerUser();
void processPendingRegistrations(const User& currentUser);
void searchAndUpdateUser();
void loginUser();
void changePassword(const string& username);



#endif 
