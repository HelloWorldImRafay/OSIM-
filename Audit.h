#ifndef AUDIT_H
#define AUDIT_H
#include <iostream>
using namespace std;
#include <string>

void logActivity(const string& username, const string& message);
void logActivity(const string& action);
void displayAudit();
void filterAuditByDate();


#endif 
