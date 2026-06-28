#include "NotificationSystem.h"
#include "NotificationLog.h"
#include "NotificationDetector.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include "User.h"
#include "Audit.h"
using namespace std;

    NotificationSystem::NotificationSystem(NotificationLog &l) : log(l) {}

    void NotificationSystem::sendNotification(User *sender, const string &type, const string &message)
    {
        int level = sender->getClearanceLevel();

        if (level < 2)
        {
            cout << "You are not authorized to send notifications.\n";
            return;
        }

        ofstream file("notifications.txt", ios::app);
        if (file.is_open())
        {
            time_t now = time(0);
            char *dt = ctime(&now);
            dt[strlen(dt) - 1] = '\0'; 

            file << sender->getUsername() << "|" << type << "|" << message << "|" << dt << "\n";
            file.close();

            cout << "[" << type << "] Notification sent by " << sender->getUsername() << "\n";

            
            logActivity("Notification sent by " + sender->getUsername() + ": [" + type + "] " + message);
        }
        else
        {
            cout << "Failed to open notifications file.\n";
        }
    }

    void NotificationSystem::displayGlobalNotifications(const std::string &username)
    {
        ifstream file("notifications.txt");
        string line;

        if (!file.is_open())
        {
            cout << "No notifications available.\n";
            return;
        }

        cout << "\n--- Global Notifications ---\n";
        while (getline(file, line))
        {
            size_t first = line.find('|');
            size_t second = line.find('|', first + 1);
            size_t third = line.find('|', second + 1);

            if (first == string::npos || second == string::npos || third == string::npos)
            {
                continue; 
            }

            string sender = line.substr(0, first);
            string type = line.substr(first + 1, second - first - 1);
            string message = line.substr(second + 1, third - second - 1);
            string timestamp = line.substr(third + 1);

            cout << "[" << type << "] from " << sender << " at " << timestamp << ":\n";
            cout << "  " << message << "\n\n";
        }
        cout << "----------------------------\n\n";
        file.close();

        logActivity("Displayed global notifications by " + username);
    }
