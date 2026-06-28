#include "AnomalyDetector.h"
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

    AnomalyDetector::AnomalyDetector()
    {
        userCount = 0;
        fill(failedLoginCount, failedLoginCount + 100, 0);
        fill(taskReassignmentCount, taskReassignmentCount + 100, 0);
        fill(createdCount, createdCount + 100, 0);
        fill(loadedCount, loadedCount + 100, 0);
        for (int i = 0; i < 1000; ++i)
        {
            createdTimestamps[i] = 0;
            loadedTimestamps[i] = 0;
        }
    }

    void AnomalyDetector::parseLogLine(const char *line, char *userId, char *action, time_t &timestamp, int &hour)
    {
        struct tm tmTime = {};
        char month[4], dayOfWeek[4];
        int day, min, sec, year;

        sscanf(line, "[%3s %3s %d %d:%d:%d %d]", dayOfWeek, month, &day, &hour, &min, &sec, &year);

        const char *timestampEnd = strchr(line, ']');
        const char *userIdStart = timestampEnd + 2;

        const char *actionStart = strchr(userIdStart, ' ');
        if (actionStart != nullptr)
        {
            size_t userIdLength = actionStart - userIdStart;
            strncpy(userId, userIdStart, userIdLength);
            userId[userIdLength] = '\0';

            strcpy(action, actionStart + 1);
        }
        else
        {
            strcpy(userId, "Unknown");
            strcpy(action, "Unknown");
        }

        tmTime.tm_hour = hour;
        tmTime.tm_min = min;
        tmTime.tm_sec = sec;
        tmTime.tm_mday = day;
        tmTime.tm_year = year - 1900;
        tmTime.tm_mon = parseMonth(month);
        tmTime.tm_isdst = -1;

        timestamp = mktime(&tmTime);
    }

    void AnomalyDetector::checkAnomalies(const char *userId, const char *action, time_t timestamp)
    {
        int userIndex = findUserIndex(userId);
        if (userIndex == -1)
        {
            addUser(userId);
            userIndex = userCount - 1;
        }

        if (strcmp(action, "Failed Authentication") == 0)
        {
            failedLoginCount[userIndex]++;
            if (failedLoginCount[userIndex] >= 3)
            {
                logAnomaly(userId, "Suspicious: 3 failed login attempts");
            }
        }

        if (strcmp(action, "Action Performed") == 0)
        {
            int actionCount = 0;
            time_t currentTime = timestamp;

            for (int i = 0; i < 1000; ++i)
            {
                if (actionHistory[userIndex][i] != 0 && difftime(currentTime, actionHistory[userIndex][i]) <= 180)
                {
                    actionCount++;
                }
            }

            if (actionCount > 50)
            {
                logAnomaly(userId, "Suspicious: More than 50 actions in 3 minutes");
            }

            actionHistory[userIndex][actionCount] = currentTime;
        }

        if (strcmp(action, "task reassignment") == 0)
        {
            taskReassignmentCount[userIndex]++;
            if (taskReassignmentCount[userIndex] >= 3)
            {
                logAnomaly(userId, "Suspicious: Task reassigned multiple times in a short period");
            }
        }

        if (strcmp(action, "login") == 0)
        {
            struct tm *loginTime = localtime(&timestamp);
            if (loginTime->tm_hour >= 0 && loginTime->tm_hour <= 6)
            {
                logAnomaly(userId, "Suspicious: Login after midnight");
            }
        }
    }

    void AnomalyDetector::logAnomaly(const char *userId, const char *message)
    {
        ofstream anomalyLog("anomaly_report.txt", ios::app);
        if (anomalyLog.is_open())
        {
            time_t now = time(0);
            char *dt = ctime(&now);
            dt[strlen(dt) - 1] = '\0';

            anomalyLog << "[" << dt << "] [ANOMALY] " << userId << ": " << message << "\n";
            anomalyLog.close();
        }
        else
        {
            cerr << "Error opening anomaly report file." << endl;
        }
    }

    void AnomalyDetector::analyzeLogs(const char *logFileName)
    {
        ifstream logFile(logFileName);
        if (!logFile.is_open())
        {
            cerr << "Error opening log file: " << logFileName << endl;
            return;
        }

        char line[256];
        char userId[50], action[100];
        time_t timestamp;
        int hour;

        while (logFile.getline(line, 256))
        {
            parseLogLine(line, userId, action, timestamp, hour);
            checkAnomalies(userId, action, timestamp);
        }

        logFile.close();
    }

    int AnomalyDetector::parseMonth(const char *month)
    {
        if (strcmp(month, "Jan") == 0)
            return 0;
        if (strcmp(month, "Feb") == 0)
            return 1;
        if (strcmp(month, "Mar") == 0)
            return 2;
        if (strcmp(month, "Apr") == 0)
            return 3;
        if (strcmp(month, "May") == 0)
            return 4;
        if (strcmp(month, "Jun") == 0)
            return 5;
        if (strcmp(month, "Jul") == 0)
            return 6;
        if (strcmp(month, "Aug") == 0)
            return 7;
        if (strcmp(month, "Sep") == 0)
            return 8;
        if (strcmp(month, "Oct") == 9)
            return 9;
        if (strcmp(month, "Nov") == 10)
            return 10;
        if (strcmp(month, "Dec") == 11)
            return 11;
        return -1;
    }

    int AnomalyDetector::findUserIndex(const char *userId)
    {
        for (int i = 0; i < userCount; ++i)
        {
            if (strcmp(users[i], userId) == 0)
            {
                return i;
            }
        }
        return -1;
    }

    void AnomalyDetector::addUser(const char *userId)
    {
        if (userCount < 100)
        {
            strncpy(users[userCount], userId, sizeof(users[userCount]));
            ++userCount;
        }
    }
