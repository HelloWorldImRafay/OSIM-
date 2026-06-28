#include "PerformanceReview.h"
#include <fstream>
#include <cstdio>


    PerformanceReview::PerformanceReview()
        : employeeName(""), tasksCompleted(0), totalTasks(0), taskScore(0), totalMessages(0) {}

    void PerformanceReview::setEmployeeName(const string &name)
    {
        employeeName = name;
    }

    void PerformanceReview::setMessageCount(int count)
    {
        totalMessages = count;
    }

    void PerformanceReview::readTasksFromFile()
    {
        ifstream file("tasks.txt");
        if (!file)
        {
            cerr << "Error: tasks.txt not found.\n";
            return;
        }

        string taskName, employeeName, assigneeName, status;
        int assigneeClearance, priority, taskTime, estimatedTime;
        long startTimestamp, endTimestamp;

        tasksCompleted = 0;
        totalTasks = 0;

        while (file >> taskName >> employeeName >> assigneeName >> assigneeClearance >> priority >> taskTime >> estimatedTime >> status >> startTimestamp >> endTimestamp)
        {
            if (employeeName == this->employeeName)
            {
                ++totalTasks;
                if (status == "Completed")
                {
                    ++tasksCompleted;
                }
                else if (status == "Expired")
                {
                    tasksCompleted += 0.5f;
                }
            }
        }
        file.close();
    }

    void PerformanceReview::updatePerformanceFile()
    {
        ifstream infile("PerformanceReview.txt");
        ofstream temp("temp.txt");
        bool updated = false;

        string name;
        float completed;
        int total, messages;
        float score;

        while (infile >> name >> completed >> total >> score >> messages)
        {
            if (name == employeeName)
            {
                temp << employeeName << ' ' << tasksCompleted << ' '
                    << totalTasks << ' ' << taskScore << ' ' << totalMessages << '\n';
                updated = true;
            }
            else
            {
                temp << name << ' ' << completed << ' '
                    << total << ' ' << score << ' ' << messages << '\n';
            }
        }

        if (!updated)
        {
            temp << employeeName << ' ' << tasksCompleted << ' '
                << totalTasks << ' ' << taskScore << ' ' << totalMessages << '\n';
        }

        infile.close();
        temp.close();

        remove("PerformanceReview.txt");
        rename("temp.txt", "PerformanceReview.txt");
    }

    float PerformanceReview::calculateScore()
    {
        readTasksFromFile();
        float messageScore = totalMessages * 0.1f;
        taskScore = (totalTasks != 0) ? (tasksCompleted / totalTasks) : 0.0f;
        taskScore += messageScore;

        updatePerformanceFile();
        return taskScore;
    }

    ostream &operator<<(ostream &out, const PerformanceReview &review)
    {
        ifstream file("PerformanceReview.txt");
        if (!file)
        {
            out << "PerformanceReview.txt not found.\n";
            return out;
        }

        string name;
        float completed, score;
        int total, messages;
        bool found = false;

        while (file >> name >> completed >> total >> score >> messages)
        {
            if (name == review.employeeName)
            {
                out << "--------------------------------------\n";
                out << "  Performance Report for: " << name << '\n';
                out << "--------------------------------------\n";
                out << " Tasks Completed (Weighted): " << completed << '\n';
                out << " Total Tasks               : " << total << '\n';
                out << " Messages Responded        : " << messages << '\n';
                out << " Total Performance Score   : " << (score * 100) << "%\n";
                out << "--------------------------------------\n";
                found = true;
                break;
            }
        }

        if (!found)
        {
            out << "No performance data found for " << review.employeeName << ".\n";
        }

        return out;
    }

    istream &operator>>(istream &in, PerformanceReview &review)
    {
        string name;
        int messages;

        cout << "Enter employee name: ";
        getline(in >> ws, name);

        cout << "Enter number of messages responded: ";
        in >> messages;

        review.setEmployeeName(name);
        review.setMessageCount(messages);
        review.calculateScore();

        cout << '\n'
            << review;
        return in;
    }
