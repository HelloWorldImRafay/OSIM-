#ifndef POLICY_ENGINE_H
#define POLICY_ENGINE_H

#include <string>
using namespace std;

class User;  

class PolicyEngine {
public:

    static bool canPromote(int fromLevel, int toLevel);
    static bool canAssignTask(int fromLevel, int toLevel);
    static bool canViewInbox(int viewerLevel, int targetLevel);
    static User* createUserByLevel(string username, string passwordHash, int level);
    
};

#endif
