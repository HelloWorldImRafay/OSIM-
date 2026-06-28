#include "MFAHandler.h"
#include "User.h"
#include <iostream>
#include <cstdlib>

using namespace std;

    string MFAHandler::generateOTP()
    {
        srand(time(0));
        int otp = 100000 + rand() % 900000;
        return to_string(otp);
    }

    bool MFAHandler::verifyOTP(User *tempUser)
    {
        string otp = generateOTP();

        Message otpMsg("System", tempUser->getUsername(), "Your OTP is: " + otp, "INFO");

        otpMsg.sendMessage(tempUser, otpMsg);

        cout << "\nOTP has been sent to your inbox.\n";

        time_t startTime = time(0);
        int attempts = 3;
        string userInput;

        while (attempts--)
        {
            int viewInbox;
            cout << "Enter 1 to view your inbox: ";
            cin >> viewInbox;
            cin.ignore();

            if (viewInbox == 1)
            {
                Message().readInbox(*tempUser);
            }

            cout << "Enter the 6-digit OTP: ";
            getline(cin, userInput);

            time_t now = time(0);
            if (difftime(now, startTime) > 30)
            {
                cout << "OTP expired. Try logging in again.\n";
                return false;
            }

            if (userInput == otp)
            {
                cout << "OTP verified successfully!\n";
                return true;
            }
            else
            {
                cout << "Incorrect OTP. " << attempts << " attempt(s) left.\n";
            }
        }

        return false;
    }
