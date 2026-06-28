#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "NotificationLog.h"
#include "NotificationSystem.h"
#include "User.h"
#include "PolicyEngine.h"
#include "Task.h"
#include "TaskManager.h"
#include "SecureMessenger.h"
#include "Audit.h"
#include "MFAHandler.h"
#include "PerformanceReview.h"
#include "AnomalyDetector.h"

using namespace std;


int main() {
    system("clear");
    User* currentUser = nullptr; 
    TaskManager taskManager;
    AnomalyDetector anomalyDetector;
    NotificationLog log;
    NotificationSystem system(log);

    int choice;
    while (true) {
        cout << "\n\t\t\t\t\t\t\t===== OSIM Unified System Menu =====\n";
        cout << "\t\t\t\t\t\t\t-------------------------------------\n";
        cout << "\t\t\t\t\t\t\t+      -----User Dashboard-----     +" << endl;
        cout << "\t\t\t\t\t\t\t|1. Register User                   |\n";
        cout << "\t\t\t\t\t\t\t|2. Process Pending Registration    |\n";
        cout << "\t\t\t\t\t\t\t|3. Login                           |\n";
        cout << "\t\t\t\t\t\t\t|4. Promote User (Executive only)   |\n";
        cout << "\t\t\t\t\t\t\t|5. Search & Update User            |\n";
        cout << endl;
        cout << endl;
        cout << "\t\t\t\t\t\t\t+  -----Task Management System----- +" << endl;
        cout << "\t\t\t\t\t\t\t|6. Add Task                        |\n";
        cout << "\t\t\t\t\t\t\t|7. Delegate Tasks                  |\n";
        cout << "\t\t\t\t\t\t\t|8. Display All Tasks               |\n";
        cout << "\t\t\t\t\t\t\t|9. Mark Task Complete              |\n";
        cout << "\t\t\t\t\t\t\t|10. Check Task Expiry              |\n";
        cout << "\t\t\t\t\t\t\t|11. Sort Tasks by Priority         |\n";
        cout << "\t\t\t\t\t\t\t|12. Save Tasks to File             |\n";
        cout << "\t\t\t\t\t\t\t|13. View My Assigned Tasks         |\n";
        cout << "\t\t\t\t\t\t\t|14. View Task Completion Reports   |\n";
        cout << endl;
        cout << endl;
        cout << "\t\t\t\t\t\t\t+   -----Secure Messaging App-----  +" << endl;
        cout << "\t\t\t\t\t\t\t|15. Send Notification              |\n";
        cout << "\t\t\t\t\t\t\t|16. Send Message                   |\n";
        cout << "\t\t\t\t\t\t\t|17. Read Inbox                     |\n";
        cout << "\t\t\t\t\t\t\t|18. Delete Message by ID           |\n";
        cout << "\t\t\t\t\t\t\t|19. List All Inbox Messages        |\n";
        cout << "\t\t\t\t\t\t\t|20. Decrypt Private Message by ID  |\n";
        cout << endl;
        cout << endl;
        cout << "\t\t\t\t\t\t\t+     -----Audit Activity-----      +" << endl;
        cout << "\t\t\t\t\t\t\t|21. View Audit Log                 |\n";
        cout << "\t\t\t\t\t\t\t|22. View Performance Review        |\n";
        cout << "\t\t\t\t\t\t\t|23. View Anomaly Report            |\n";
        cout << "\t\t\t\t\t\t\t|24. Logout                         |\n";
        cout << "\t\t\t\t\t\t\t|25. Exit Program                   |\n";
        cout << "\t\t\t\t\t\t\t------------------------------------\n";

        cout << endl;
        cout << "\t\t\t\t\t\t\t\t+-------------------+" << endl;
        cout << "\t\t\t\t\t\t\t\t   Enter choice: ";
        cin >> choice;
        cin.ignore();  // Clear input buffer

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (currentUser != nullptr)
                    processPendingRegistrations(*currentUser);
                else
                    cout << "Please login first to access this feature.\n";
                break;
            case 3: {
                if (currentUser) {
                    cout << "Already logged in as: " << currentUser->getUsername() << "\n";
                    break;
                }

                string username, password;
                cout << "Username: ";
                getline(cin, username);
                cout << "Password: ";
                getline(cin, password);

                User* tempUser = User::loginUser(username, password);
                if (!tempUser) {
                    cout << "Login failed. Incorrect credentials.\n";
                    break;
                }

                if (MFAHandler::verifyOTP(tempUser)) {
                    currentUser = tempUser;
                    if (currentUser) {
                        // Now load the tasks after successful login
                        taskManager.loadTasksFromFile("tasks.txt", currentUser);
                        system.displayGlobalNotifications(currentUser->getUsername());
                    }
                } else {
                    delete tempUser;
                }

                break;
            }
            case 4:
            if (currentUser->getClearanceLevel() == 4) {  // Check if user is Executive or higher
                currentUser->promoteUser(currentUser);  // Call promoteUser
            } else {
                cout << "You do not have permission to promote users.\n";
            }
            break;
            case 5:
                searchAndUpdateUser();
                break;
                case 6: {
                    if (currentUser == nullptr) {
                        cout << "No user is currently logged in.\n";
                        break;
                    }
                
                    string name, assignee;
                    int priority, progress;
                    long ttl;
                
                    cout << "Enter task name: ";
                    getline(cin, name);
                
                    cout << "Enter assignee username: ";
                    getline(cin, assignee);
                
                    // Check if assignee exists and fetch clearance
                    int assigneeLevel;
                    if (!User::userExists(assignee, &assigneeLevel)) {
                        cout << "Assignee does not exist. Task not added.\n";
                        break;
                    }
                
                    // Enforce clearance level rule
                    if (currentUser->getClearanceLevel() <= assigneeLevel) {
                        cout << "Permission denied: You cannot assign tasks to users with equal or higher clearance.\n";
                        break;
                    }
                
                    cout << "Enter priority (1-10): ";
                    cin >> priority;
                    cout << "Enter time-to-live (in seconds): ";
                    cin >> ttl;
                    cin.ignore();
                
                    // Create task
                    Task* newTask = new Task(name, currentUser->getUsername(), assignee, assigneeLevel, priority, ttl, 0);
                    taskManager.addTask(newTask, currentUser);
                
                    break;
                }
                case 7: {

                    if (currentUser == nullptr) {
                        cout << "You must be logged in to delegate a task.\n";
                    } else {
                        taskManager.delegateTask(taskManager, currentUser);  // Delegate task only if currentUser is not null
                    }
                    break;
                }
        
                
            case 8:
                taskManager.displayAllTasks();
                break;
            case 9: {
                string tname;
                string username; 
                cout << "Enter task name to mark complete: ";
                getline(cin, tname);
                cout << "Enter your username: "; 
                getline(cin, username); 
                taskManager.markTaskComplete(tname, username); 
                break;
            }
            case 10:
            taskManager.checkExpiryRecursive();
            cout << "Expiry checked for all tasks.\n";
            break;
            case 11:
                taskManager.sortTasksByPriority();
                break;
            case 12:
                taskManager.saveAllTasksToFile("tasks.txt");
                break;
            case 13: { // View My Assigned Tasks
                if (!currentUser) {
                    cout << "Please login first.\n";
                    break;
                }
            
                taskManager.viewTasksForUser(currentUser->getUsername());
                break;
            }
            case 14: 
            taskManager.generateCompletionReport();
            break;
            case 15: {
                if (!currentUser) {
                    cout << "Please login first.\n";
                    break;
                }
                string type, message;
                cout << "Enter notification type (INFO, WARNING, EMERGENCY): ";
                cin >> type;
                cin.ignore();
                cout << "Enter message: ";
                getline(cin, message);

                if (currentUser->getClearanceLevel() < 2) {
                    cout << "You are not authorized to send notifications.\n";
                    break;
                }

                system.sendNotification(currentUser, type, message);
                break;
            }

            case 16: {
                if (!currentUser) {
                    cout << "Please login first.\n";
                    break;
                }
                string type, recipient = "", content;
                cout << "Message Type (INFO / PRIVATE / ALERT): ";
                cin >> type;
                cin.ignore();

                cout << "Content: ";
                getline(cin, content);

                if (type == "INFO" || type == "PRIVATE") {
                    cout << "Recipient Username: ";
                    cin >> recipient;
                    cin.ignore();

                    int recipientLevel = 0;
                    if (!User::userExists(recipient, &recipientLevel)) {
                        cout << "Recipient does not exist.\n";
                        break;
                    }

                    Message msg(currentUser->getUsername(), recipient, content, type);
                    if (!msg.canSendMessage(currentUser, msg)) {
                        cout << "Permission denied: You can't send this type of message.\n";
                        break;
                    }

                    if (type == "PRIVATE") msg.encrypt();
                    msg.sendMessage(currentUser, msg);

                } else if (type == "ALERT") {
                    if (currentUser->getClearanceLevel() < 3) {
                        cout << "Only Director or Executive can send ALERT.\n";
                        break;
                    }

                    Message msg(currentUser->getUsername(), "", content, "ALERT");
                    msg.sendMessage(currentUser, msg);
                } else {
                    cout << "Invalid message type.\n";
                }
                break;
            }

            case 17:
                if (!currentUser) {
                    cout << "Please login first.\n";
                    break;
                }
                Message().readInbox(*currentUser);
                break;

            case 18: {
                if (!currentUser) {
                    cout << "Please login first.\n";
                    break;
                }
                int msgID;
                cout << "Enter Message ID to delete: ";
                cin >> msgID;
                cin.ignore();
                Message::deleteMessage(msgID, currentUser->getUsername());
                break;
            }

            case 19:
                if (!currentUser) {
                    cout << "Please login first.\n";
                    break;
                }
                Message().readInbox(*currentUser); 
                break;

            case 20: {
                if (!currentUser) {
                    cout << "Please login first.\n";
                    break;
                }
                int msgID;
                cout << "Enter PRIVATE Message ID to decrypt: ";
                cin >> msgID;
                cin.ignore();
                Message().decryptPrivateMessageByID(msgID, currentUser->getUsername());
                break;
            }

            case 21:
                displayAudit();
                break;
            
            case 22: { 
                if (!currentUser) {
                    cout << "Please login first.\n";
                    break;
                }
                PerformanceReview review;
                review.setEmployeeName(currentUser->getUsername()); 
                review.calculateScore(); 
                cout << review;  
                break;
            }
    
            case 23: {
                if (!currentUser) {
                    cout << "Please login first.\n";
                    break;
                }
                cout << "Anomaly Detection is Starting..." << endl;
                cout << "Check Anomaly Report" << endl;
                break;
            }
            case 24: 
                if (currentUser) {
                    delete currentUser;
                    currentUser = nullptr;
                    cout << "Logged out successfully.\n";
                } else {
                    cout << "No user is currently logged in.\n";
                }
                break;

            case 25: 
                if (currentUser) delete currentUser;
                cout << "Exiting program.\n";
                return 0;

            default:
                cout << "Invalid choice.\n";
        }
    
    }
    return 0;
}