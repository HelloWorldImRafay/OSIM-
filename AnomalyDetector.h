#ifndef ANOMALYDETECTOR_H
#define ANOMALYDETECTOR_H

#include <ctime>

class AnomalyDetector {
private:
    char users[100][50];
    int failedLoginCount[100];
    int taskReassignmentCount[100];
    time_t actionHistory[100][1000];
    int userCount;

    int createdCount[100];
    int loadedCount[100];
    time_t createdTimestamps[1000];
    time_t loadedTimestamps[1000];

public:
    AnomalyDetector();

    void parseLogLine(const char* line, char* userId, char* action, time_t& timestamp, int& hour);
    void checkAnomalies(const char* userId, const char* action, time_t timestamp);
    void logAnomaly(const char* userId, const char* message); 
    void analyzeLogs(const char* logFileName);

private:
    int parseMonth(const char* month);
    int findUserIndex(const char* userId);
    void addUser(const char* userId);
};

#endif
