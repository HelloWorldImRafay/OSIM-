// Notification.h
#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>
using namespace std;

class Notification {
    string message;
    string type;     
    string sender;

public:
    Notification();
    Notification(string msg, string t, string snd);
    void display() const;
    string getType() const;
};

#endif
