#include "Task.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
using namespace std;


    
    Task::Task(const string& n, const string& c, const string& a, int ass_clear, int p, long ttl , int prog)
        : name(n), creator(c), assignee(a), assigneeClearance(ass_clear), priority(p), ttlSeconds(ttl), progressPercent(prog), status("Created") {
        creationTime = time(0);
        lastModifiedTime = creationTime; 
    }



    void Task::checkExpiry() {
        long currentTime = time(0);
    
        if (status == "Completed" || status == "Expired")
            return; 
    
        if ((currentTime - creationTime) >= ttlSeconds) {
            status = "Expired";
            std::cout << "Task '" << name << "' has expired.\n";
        }
    }

    

    void Task::displayTask() {
        cout << "Task: " << name << " | Creator: " << creator 
            << " | Assigned to: " << assignee << " | Priority: " << priority
            << " | Status: " << status 
            << " | Progress: " << progressPercent << "%" << endl;

    }

    string Task ::getname()const{return name;}
    string Task::getAssignee() const {
        return assignee;
    }

    long Task::getCreationTime() const {
        return creationTime;
    }

    long Task::getTTL() const {
        return ttlSeconds;
    }

    string Task::getStatus() const {
        return status;
    }

    void Task :: setAssigneeClearance(int c){
        assigneeClearance = c;
    }

    void Task::updateProgress(int percent) {
        if (percent >= 0 && percent <= 100) {
            progressPercent = percent;
    
            if (progressPercent == 100) {
                markComplete(assignee);  
            } else if (progressPercent > 0) {
                status = "InProgress";
            } else {
                status = "Assigned";  
            }
    
        } else {
            cout << "Invalid progress value. Must be between 0 and 100.\n";
        }
    }
    

    int Task :: getProgress()const{
        return progressPercent;
    }
    
    int Task :: getAssigneeClearance()const{
        return assigneeClearance;
    }

    void Task::setStatus(const string& newStatus) {
        if (newStatus == "Created" || newStatus == "Assigned" || 
            newStatus == "InProgress" || newStatus == "Completed" || 
            newStatus == "Expired") {
            
            status = newStatus;
    
            if (newStatus == "Completed") {
                progressPercent = 100;
            } else if (newStatus == "Created" || newStatus == "Assigned") {
                progressPercent = 0;
            }
    
        } else {
            cout << "Invalid status. Allowed: Created, Assigned, InProgress, Completed, Expired.\n";
        }
    }

    bool Task::delegateTask(const string& newAssignee, int newAssigneeClearance) {
        if (newAssigneeClearance >= assigneeClearance) {
            assignee = newAssignee;
            assigneeClearance = newAssigneeClearance;
            status = "Assigned";  
            cout << "Task has been reassigned to " << newAssignee << endl;
            return true;
        } else {
            cout << "Cannot delegate task to someone with lower clearance level.\n";
            return false;
        }
    }

    void Task::saveToFile(ofstream &file)
    {
    file << name << "," << creator << "," << assignee << "," << assigneeClearance << ","
            << priority << "," << ttlSeconds << "," << progressPercent << "," << status << ","
            << creationTime << "," << lastModifiedTime << "\n";



        /*ofstream file("performanceTask.txt", ios::app);
    if (file.is_open()) {
        file << creator << " " << progressPercent << endl;
        file.close();

    } else {
        cout << "Error writing to performanceTask.txt\n";
    }
       */ 
    }

    Task* Task::loadFromFile(ifstream& file) {
        string line;
        if (!getline(file, line)) return nullptr;
    
        size_t pos = 0, next;
        string fields[10];
        int index = 0;
    
        while ((next = line.find(',', pos)) != string::npos && index < 9) {
            fields[index++] = line.substr(pos, next - pos);
            pos = next + 1;
        }
        fields[index] = line.substr(pos); 
    
        if (index < 9) return nullptr; 
    
        
        string name = fields[0];
        string creator = fields[1];
        string assignee = fields[2];
        int clearance = stoi(fields[3]);
        int priority = stoi(fields[4]);
        long ttl = stol(fields[5]);
        int progress = stoi(fields[6]);
        string status = fields[7];
        long creationTime = stol(fields[8]);
        long lastModified = stol(fields[9]);
    
        
        Task* t = new Task(name, creator, assignee, clearance, priority, ttl, progress);
        t->setStatus(status);
        t->setCreationTime(creationTime);
        t->setLastModifiedTime(lastModified);
        return t;
    }
    
    
    void Task::setCreationTime(long time) { creationTime = time; }
    void Task::setLastModifiedTime(long time) { lastModifiedTime = time; }
    void Task::setAssigne(const string& username) {
        this->assignee = username;  // Assuming 'assignee' is a member variable
    }

    void Task::markComplete(const string& username) {
        if (assignee == username) {
            status = "Completed";
            lastModifiedTime = time(0);  // Update completion time
            cout << "Task '" << name << "' has been completed by " << assignee << ".\n";
        } else {
            cout << "You are not authorized to complete this task.\n";
        }
    }
    
    
    void Task::logCompletion() {
        long completionTime = time(0);
        
        char buffer[80];
        struct tm* timeInfo = localtime(&completionTime);
        
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
        
        cout << "Task '" << name << "' has been completed at " << buffer << ".\n";
    }
    
    bool Task::isOverdue() const {
        long now = time(0);
        return (now > creationTime + ttlSeconds) && status != "Completed";
    }

    void Task::editTask(const string& newName, int newPriority, long newTTL, const string& newStatus) {
        name = newName;
        priority = newPriority;
        ttlSeconds = newTTL;
        setStatus(newStatus);
        lastModifiedTime = time(0); 
    }

    long Task :: getLastModifiedTime()const{ return lastModifiedTime; }

    bool Task::operator<(const Task& other) const {
        return this->priority < other.priority;  
    }

    string Task :: getCreator()const{return creator;}
    int Task :: getPriorty()const{return priority;}

    bool Task::isCompleted() const {
        return status == "Completed";
    }

    
    
    
    
