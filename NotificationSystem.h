// NotificationSystem.h
#ifndef NOTIFICATIONSYSTEM_H
#define NOTIFICATIONSYSTEM_H

#include "NotificationLog.h"
#include "User.h"  
using namespace std;

class NotificationSystem {
    NotificationLog& log;  

public:
    NotificationSystem(NotificationLog& l);
    void displayGlobalNotifications(const std::string& username);

    void sendNotification(User* sender, const string& type, const string& message);

};
#endif
