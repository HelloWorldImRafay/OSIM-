#ifndef MFA_HANDLER_H
#define MFA_HANDLER_H
#include <iostream>
#include <string>
#include <ctime>
#include "User.h"
#include "SecureMessenger.h"

using namespace std;

class MFAHandler {
public:
    static bool verifyOTP(User* tempUser);
    static string generateOTP();
};

#endif
