#ifndef NOTIFICATIONLOG_H
#define NOTIFICATIONLOG_H

#include "NotificationDetector.h"

class NotificationLog {
    Notification* notifications;
    int capacity;
    int count;

    void resize(); 

public:
    NotificationLog();
    ~NotificationLog();

    void addNotification(const Notification& n);
    void showAll() const;
};

#endif
