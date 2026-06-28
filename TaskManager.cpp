#include "TaskManager.h"
#include "User.h"
#include "Task.h"
#include "Audit.h"
#include "PolicyEngine.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
using namespace std;

    TaskManager::TaskManager()
    {
        taskArray = nullptr;
        taskCount = 0;
    }

    TaskManager::~TaskManager()
    {

        for (int i = 0; i < taskCount; i++)
        {
            delete taskArray[i];
        }
        delete[] taskArray;
    }

    void TaskManager::addTask(Task *newTask, User *currentUser)
    {
        string role = currentUser->getRole(); // Assuming getRole() returns "Junior", "Manager", etc.

        if (role == "Junior")
        {
            logActivity("Unauthorized Action" , currentUser->getUsername());
            cout << "You are not authorized to create tasks.\n";
            return;
        }

        
        Task **tempArray = new Task *[taskCount + 1];

        for (int i = 0; i < taskCount; i++)
        {
            tempArray[i] = taskArray[i];
        }

        tempArray[taskCount] = newTask;

        delete[] taskArray;
        taskArray = tempArray;
        taskCount++;

        logActivity("Action Performed" , currentUser->getUsername());
    }


    void TaskManager::viewTasksForUser(const string& username) const {
        bool found = false;
        for (int i = 0; i < taskCount; ++i) {
            if (taskArray[i]->getAssignee() == username) {
                cout << "-----------------------------\n";
                cout << "Task Name: " << taskArray[i]->getname() << "\n";
                cout << "Creator: " << taskArray[i]->getCreator() << "\n";
                cout << "Priority: " << taskArray[i]->getPriorty() << "\n";
                cout << "Progress: " << taskArray[i]->getProgress() << "%\n";
                cout << "TTL (seconds): " << taskArray[i]->getTTL() << "\n";
                cout << "-----------------------------\n";
                found = true;
            }
        }
        if (!found) {
            cout << "No tasks assigned to user: " << username << "\n";
        }
    }
    
    

    void TaskManager::removeTask(const string &taskName)
    {
        int taskIndex = -1;

        for (int i = 0; i < taskCount; i++)
        {
            if (taskArray[i]->getname() == taskName)
            {
                taskIndex = i;
                break;
            }
        }

        if (taskIndex == -1)
        {
            cout << "Task not found.\n";
            return;
        }

        delete taskArray[taskIndex];

        for (int i = taskIndex; i < taskCount - 1; i++)
        {
            taskArray[i] = taskArray[i + 1];
        }

        taskCount--;
        logActivity("Action Performed" , taskName);
    }

    void TaskManager::displayAllTasks()
    {
        if (taskCount == 0)
        {
            cout << "No tasks available.\n";
            return;
        }

        for (int i = 0; i < taskCount; i++)
        {
            taskArray[i]->displayTask();
        }
    }

    void TaskManager::markTaskComplete(const string& taskName, const string& username)
    {
    bool found = false;

    for (int i = 0; i < taskCount; i++)
    {
        if (taskArray[i]->getname() == taskName)
        {
            taskArray[i]->markComplete(username);  // Pass the username to markComplete
            found = true;
            logActivity("Task completed:" , username);
            break;
        }
    }

    if (!found)
    {
        cout << "Task not found.\n";
    }
    }


    void TaskManager::checkTaskExpiry() {
        for (int i = 0; i < taskCount; i++) {
            taskArray[i]->checkExpiry();
        }
    }
    

    void TaskManager::loadTasksFromFile(const string& filename, User* currentUser) {
        ifstream fin(filename);
        if (!fin.is_open()) {
            cout << "Could not open file: " << filename << "\n";
            return;
        }
    
        while (fin) {
            string name, creator, assignee, status, clearance, priority, ttl, progress, creationTime, lastModified;
    
            if (!(getline(fin, name, ',') &&
                  getline(fin, creator, ',') &&
                  getline(fin, assignee, ',') &&
                  getline(fin, clearance, ',') &&
                  getline(fin, priority, ',') &&
                  getline(fin, ttl, ',') &&
                  getline(fin, progress, ',') &&
                  getline(fin, status, ',') &&
                  getline(fin, creationTime, ',') &&
                  getline(fin, lastModified))) {
                break;
            }
    
            int clearanceLevel = stoi(clearance);
            int priorityLevel = stoi(priority);
            long timeToLive = stol(ttl);
            int progressLevel = stoi(progress);
            long creationTimeLong = stol(creationTime);
            long lastModifiedLong = stol(lastModified);
    
            Task* t = new Task(name, creator, assignee, clearanceLevel, priorityLevel, timeToLive, progressLevel);
            t->setStatus(status);
            t->setCreationTime(creationTimeLong);
            t->setLastModifiedTime(lastModifiedLong);
    
            addTask(t, currentUser);  // Adds the task with role-based checks
            logActivity("Action Performed" , t->getname());
        }
    
        fin.close();
        checkExpiryRecursive();
        logActivity("Action Performed" , filename);
    }

    void TaskManager::saveAllTasksToFile(const string &filename)
    {
        ofstream fout(filename);
        if (!fout.is_open())
        {
            cout << "Could not open file for saving: " << filename << "\n";
            return;
        }

        for (int i = 0; i < taskCount; i++)
        {
            taskArray[i]->saveToFile(fout);
        }

        fout.close();
        logActivity("Action Performed" , filename);
    }

    Task *TaskManager::getTaskAt(int index) const
    {
        if (index >= 0 && index < taskCount)
            return taskArray[index];
        return nullptr;
    }

    void TaskManager::generateCompletionReport() const {
        cout << "================== Completion Report ==================\n";
        for (int i = 0; i < taskCount; ++i) {
            if (taskArray[i]->getStatus() == "Completed") {
                long completionTime = taskArray[i]->getLastModifiedTime(); 
    
                char buffer[80];
                struct tm* timeInfo = localtime(&completionTime);
                strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
    
                // Display task details
                cout << "Task: " << taskArray[i]->getname() 
                     << " | Creator: " << taskArray[i]->getCreator() 
                     << " | Assignee: " << taskArray[i]->getAssignee()
                     << " | Completion Time: " << buffer << endl;
            }
        }
        cout << "=========================================================\n";
    }

    void TaskManager::checkExpiryRecursive() {
        checkExpiryRecursiveHelper(0);
    }
    
    void TaskManager::checkExpiryRecursiveHelper(int index) {
        if (index >= taskCount) return;
    
        taskArray[index]->checkExpiry();
        logActivity("Action Performed" , taskArray[index]->getname());
    
        checkExpiryRecursiveHelper(index + 1);
    }
    

    int TaskManager ::getTaskCount() const { return taskCount; }

    void TaskManager::sortTasksByPriority()
    {
        for (int i = 0; i < taskCount - 1; ++i)
        {
            for (int j = i + 1; j < taskCount; ++j)
            {
                if (*taskArray[j] < *taskArray[i])
                {
                    Task *temp = taskArray[i];
                    taskArray[i] = taskArray[j];
                    taskArray[j] = temp;
                }
            }
        }
        cout << "Tasks sorted by priority.\n";
    }

    Task* TaskManager::getTaskName(const string& taskName) {
        for (int i = 0; i < taskCount; ++i) {
            if (taskArray[i] && taskArray[i]->getname() == taskName) {
                return taskArray[i];  // Return the task if found
            }
        }
        cout << "Task not found: " << taskName << endl;
        return nullptr; 
    }
    
    
    void TaskManager ::  delegateTaskRecursive(TaskManager& manager, User* currentUser, const string& taskName, const string& newAssignee) {
        Task* task = manager.getTaskName(taskName);
        if (!task) {
            cout << "Task not found.\n";
            return;
        }
    
        if (task->getAssignee() != currentUser->getUsername()) {
            cout << "You can only delegate tasks assigned to you.\n";
            return;
        }
    
        int assigneeLevel = 0;
        if (!User::userExists(newAssignee, &assigneeLevel)) {
            cout << "User does not exist.\n";
            return;
        }
    
        if (assigneeLevel > currentUser->getClearanceLevel()) {
            cout << "You cannot delegate to someone with higher clearance.\n";
            return;
        }
    
        
        task->setAssigne(newAssignee);
        logActivity(currentUser->getUsername(), "delegated task '" + taskName + "' to " + newAssignee);
        cout << "Task successfully delegated to " << newAssignee << ".\n";
    }
    
    void TaskManager::delegateTask(TaskManager& manager, User* currentUser) {
        if (!currentUser) {
            cout << "Please login first.\n";
            return;
        }
    
        string taskName, newAssignee;
        cout << "Enter task name to delegate: ";
        getline(cin, taskName);
    
        cout << "Enter new assignee username: ";
        getline(cin, newAssignee);
    
        ifstream inFile("User.txt");
        string uname, pass;
        int level;
        bool found = false;
    
        while (inFile >> uname >> pass >> level) {
            if (uname == newAssignee) {
                found = true;
    
                if (level > currentUser->getClearanceLevel()) {
                    cout << "Delegation allowed only to users with equal or higher clearance.\n";
                    inFile.close();
                    return;
                }
                break;
            }
        }
    
        inFile.close();
    
        if (!found) {
            cout << "New assignee not found.\n";
            return;
        }
    
        delegateTaskRecursive(manager, currentUser, taskName, newAssignee);
    }

    Task** TaskManager::getTaskArray() const {
        return taskArray;
    }

   
   
    
   