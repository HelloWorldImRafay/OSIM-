#ifndef SECUREMESSENGER_H
#define SECUREMESSENGER_H
#include <iostream>
using namespace std;
#include <string>
#include "User.h"

class Message {
private:
    int id;
    string sender;
    string recipient;
    string content;
    string type;
    bool isRead;

    static int lastMessageID;

public:
    Message();
    Message(const string& s, const string& r, const string& c, const string& t);

    int getID() const;
    string getSender() const;
    string getRecipient() const;
    string getContent() const;
    string getType() const;

    void encrypt();
    void decrypt();

    void display() const;

    int getClearanceLevel(const string& role);
    bool canSendMessage(const User* sender, const Message& msg);
    void saveToInbox();
    void sendMessage(User* sender, Message& msg);
    void readInbox(const User& currentUser);
    static void deleteMessage(int messageID, const string& username);
    static void listInboxMessages(const string& username);
    void decryptPrivateMessageByID(int msgID, const string& recipient);
    void readLatestOTP(const string& recipientUsername);


};

#endif
