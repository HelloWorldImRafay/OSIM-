#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Task.h"
#include "User.h"

#include <string>

class TaskManager {
private:

    Task** taskArray;  
    int taskCount;
    int count;
    void checkExpiryRecursiveHelper(int index);

public:
    
    TaskManager();
    ~TaskManager();
    Task* getTaskName(const std::string& taskName); 
    void addTask(Task* newTask, User* currentUser);
    void viewTasksForUser(const string& username) const;
    void removeTask(const string& taskName);  
    void delegateTaskRecursive(TaskManager& manager, User* currentUser, const string& taskName, const string& newAssignee);
    void delegateTask(TaskManager& manager, User* currentUser);
    void displayAllTasks();  
    void markTaskComplete(const string& taskName);
    void checkTaskExpiry();
    void loadTasksFromFile(const std::string &filename, User* currentUser);
    void saveAllTasksToFile(const string& filename);
    Task* getTaskAt(int index) const;
    void checkExpiryRecursive();
    int getTaskCount() const;
    void sortTasksByPriority();
    Task** getTaskArray() const;
    void generateCompletionReport() const;
    void markTaskComplete(const string& taskName, const string& username);

    




};

#endif // TASKMANAGER_H
