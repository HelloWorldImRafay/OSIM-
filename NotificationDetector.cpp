// Notification.cpp
#include "NotificationDetector.h"
#include <iostream>
using namespace std;

    Notification::Notification() {
    message = "";
    type = "";
    sender = "";
    }

    Notification::Notification(string msg, string t, string snd) {
    message = msg;
    type = t;
    sender = snd;
    }

    void Notification::display() const {
    cout << "[" << type << "] from " << sender << ": " << message << endl;
    }

    string Notification::getType() const {
    return type;
    }
