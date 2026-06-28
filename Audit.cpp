#include "Audit.h"
#include <fstream>
#include <ctime>
#include <cstring>
#include <iostream>
using namespace std;

    void logActivity(const string& action) {
        ofstream auditFile("audit.txt", ios::app);
        if (auditFile.is_open()) {
            time_t now = time(0);
            char* dt = ctime(&now); 
            dt[strlen(dt) - 1] = '\0';  

            auditFile << "[" << dt << "] " << action << "\n";
            auditFile.close();
        }
    }

    void logActivity(const string& username, const string& action) {
        ofstream auditFile("audit.txt", ios::app);
        if (auditFile.is_open()) {
            time_t now = time(0);
            char* dt = ctime(&now);
            dt[strlen(dt) - 1] = '\0';

            auditFile << dt << "|" << username << "|" << action << "\n";
            auditFile.close();
        }
    }

    void displayAudit() {
        ifstream auditFile("audit.txt");
        if (!auditFile.is_open()) {
            cout << "No audit log found.\n";
            return;
        }

        cout << "\n--- Audit Log ---\n";
        string line;
        while (getline(auditFile, line)) {
            cout << line << "\n";
        }
        cout << "-----------------\n";

        auditFile.close();
    }

    void filterAuditByDate() {
        ifstream auditFile("audit.txt");
        if (!auditFile.is_open()) {
            cout << "No audit log found.\n";
            return;
        }

        string inputDate;
        cout << "Enter date to filter (format: YYYY-MM-DD): ";
        cin >> inputDate;

        string year = inputDate.substr(0, 4);
        string month = inputDate.substr(5, 2);
        string day = inputDate.substr(8, 2);

        const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
        int monthIndex = stoi(month) - 1;
        if (monthIndex < 0 || monthIndex > 11) {
            cout << "Invalid month entered.\n";
            return;
        }
        string monthAbbrev = months[monthIndex];

        cout << "\n--- Filtered Audit Log (" << monthAbbrev << " " << day << " " << year << ") ---\n";

        string line;
        bool found = false;

        while (getline(auditFile, line)) {
            string logDate = line.substr(4, 12); 

            if (logDate == (monthAbbrev + " " + day + " " + year)) {
                cout << line << "\n";
                found = true;
            }
        }

        if (!found) {
            cout << "No logs found for that date.\n";
        }

        cout << "-------------------------------\n";

        auditFile.close();
    }
