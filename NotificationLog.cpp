// NotificationLog.cpp
#include "NotificationLog.h"
#include <iostream>
using namespace std;

    NotificationLog::NotificationLog() {
    capacity = 10;
    count = 0;
    notifications = new Notification[capacity];
    }

    NotificationLog::~NotificationLog() {
    delete[] notifications;
    }

    void NotificationLog::resize() {
    int newCapacity = capacity * 2;
    Notification* newArray = new Notification[newCapacity];
    for (int i = 0; i < count; i++) {
        newArray[i] = notifications[i];
    }
    delete[] notifications;
    notifications = newArray;
    capacity = newCapacity;
    }

    void NotificationLog::addNotification(const Notification& n) {
    if (count == capacity) {
        resize();
    }
    notifications[count++] = n;
    }

    void NotificationLog::showAll() const {
    for (int i = 0; i < count; i++) {
        notifications[i].display();
    }
    }
