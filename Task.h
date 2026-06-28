#ifndef TASK_H
#define TASK_H

#include <string>
using namespace std;

class Task {
    string name;
    string creator;
    string assignee;
    int assigneeClearance; 
    int priority;
    long creationTime;
    long ttlSeconds;
    string status;
    int progressPercent;
    long lastModifiedTime;

public:
    Task(const string& name, const string& creator, const string& assignee, int assigneeClearence, int priority, long ttl , int prog);
    ~Task() {}
    void checkExpiry();
    void markComplete(const string& username);
    void displayTask();
    int getPriorty()const;
    string getCreator()const;
    int getAssigneeClearance()const;         
    void setAssigneeClearance(int c); 
    void updateProgress(int percent);
    int getProgress() const;
    void setAssigne(const string& username);
    string getname()const;
    string getAssignee() const;
    long getCreationTime() const;
    long getTTL() const;
    string getStatus() const;
    void setStatus(const string& newStatus);
    bool delegateTask(const string& newAssignee, int newAssigneeClearance);
    void saveToFile(ofstream& file);
    static Task* loadFromFile(ifstream& file);
    bool isOverdue() const;
    void editTask(const string& newName, int newPriority, long newTTL, const string& newStatus);
    long getLastModifiedTime() const;
    void setCreationTime(long time);
    void setLastModifiedTime(long time);
    bool operator<(const Task& other) const;
    void logCompletion();


    bool isCompleted() const; 



};

#endif
