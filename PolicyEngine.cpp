#include "PolicyEngine.h"
#include "User.h"    

    bool PolicyEngine::canPromote(int fromLevel, int toLevel) {
        return fromLevel < toLevel;
    }

    bool PolicyEngine::canAssignTask(int fromLevel, int toLevel) {
        return fromLevel > toLevel;
    }


    bool PolicyEngine::canViewInbox(int viewerLevel, int targetLevel) {
        return viewerLevel >= targetLevel;
    }

    User* PolicyEngine::createUserByLevel(string username, string passwordHash, int level) {
        switch (level) {
            case 1: return new Junior(username, passwordHash, level);
            case 2: return new Manager(username, passwordHash, level);
            case 3: return new Director(username, passwordHash, level);
            case 4: return new Executive(username, passwordHash, level);
            default: return nullptr;
        }
    }
