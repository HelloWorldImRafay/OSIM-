#ifndef PERFORMANCE_REVIEW_H
#define PERFORMANCE_REVIEW_H

#include <iostream>
#include <string>
using namespace std;

class PerformanceReview {
private:
    string employeeName;
    float tasksCompleted;
    int totalTasks;
    float taskScore;
    int totalMessages;
    void readTasksFromFile();
    void updatePerformanceFile();

public:
    PerformanceReview();
    void setEmployeeName(const string &name);
    void setMessageCount(int count); 
    float calculateScore(); 
    friend ostream &operator<<(ostream &out, const PerformanceReview &review);
    friend istream &operator>>(istream &in, PerformanceReview &review);
};

#endif
