#include "SecureMessenger.h"
#include <iostream>
#include <fstream>
using namespace std;
#include "User.h"
#include "Audit.h"


int Message::lastMessageID = 0;

    Message::Message() {
        ifstream idFile("last_id.txt");
        if (idFile.is_open()) {
            idFile >> lastMessageID;
            idFile.close();
        } else {
            lastMessageID = 0;  
        }
    
        id = ++lastMessageID;  
        ofstream outFile("last_id.txt");  
        outFile << lastMessageID;
        outFile.close();
    
        sender = "";
        recipient = "";
        content = "";
        type = "";
        isRead = false;
    }
    

    Message::Message(const string& s, const string& r, const string& c, const string& t)
        : id(++lastMessageID), sender(s), recipient(r), content(c), type(t) {}

    int Message::getID() const { return id; }
    string Message::getSender() const { return sender; }
    string Message::getRecipient() const { return recipient; }
    string Message::getContent() const { return content; }
    string Message::getType() const { return type; }

    void Message::encrypt() {
        if (type == "PRIVATE") {
            for (char& ch : content) {
                if (ch >= 32 && ch <= 126) {
                    ch = 32 + ((ch - 32 + 3) % 95);  
                }
            }
        }
    }
    

    void Message::decrypt() {
        for (char& ch : content) {
            if (ch >= 32 && ch <= 126) {
                ch = 32 + ((ch - 32 - 3 + 95) % 95);
            }
        }
    }
    

    void Message::display() const {
        cout << "[ID: " << id << "] "
                << "From: " << sender << ", To: " << recipient << "\n"
                << "Type: " << type << "\n"
                << "Content: " << content << "\n";
    }

    int Message :: getClearanceLevel(const string& role) {
        if (role == "Junior") return 1;
        if (role == "Manager") return 2;
        if (role == "Director") return 3;
        return 0; 
    }
    
    bool Message::canSendMessage(const User* sender, const Message& msg) {
        int clearance = sender->getClearanceLevel();
        string msgType = msg.getType();
    
        if (msgType == "ALERT") {
            return clearance >= 3;  
        } 
        else if (msgType == "PRIVATE") {
            return clearance >= 1; 
        }
    
        return true;
    }
                

    void Message::saveToInbox() {
        string inboxFileName = recipient + "_inbox.txt";
        ofstream inboxFile(inboxFileName, ios::app);
    
        if (inboxFile.is_open()) {
            inboxFile << "Message ID: " << id << endl;
            inboxFile << "From: " << sender << endl;
            inboxFile << "Type: " << type << endl;
            inboxFile << (isRead ? "[Read]" : "[Unread]") << endl;
            inboxFile << "Content: " << (type == "PRIVATE" ? "[Encrypted]" : content) << endl;
            inboxFile << "----------------------------------\n";
            inboxFile.close();
        }
    
        if (type == "PRIVATE") {
            ofstream privStore(recipient + "_private_store.txt", ios::app);
    
            if (privStore.is_open()) {
                privStore << "ID:" << id << "|CONTENT:" << content << "\n";
                privStore.close();
            }
        }
        logActivity("Message saved to inbox for " + recipient + " [ID: " + to_string(id) + "]");
    }
    
    
    void Message::sendMessage(User* sender, Message& msg) {
        if (!canSendMessage(sender, msg)) {
            cout << "You don't have permission to send this message." << endl;
            return;
        }
    
        if (msg.getType() == "ALERT") {
            ifstream file("User.txt");
            string uname, hashedPass;
            int level;
    
            while (file >> uname >> hashedPass >> level) {
                Message userMsg = msg;  
                userMsg.recipient = uname;
                userMsg.saveToInbox();
            }
    
            file.close();
            cout << "ALERT message broadcasted to all users.\n";
        } else {
            ifstream file("User.txt");
            string uname, hashedPass;
            int level;
            bool found = false;
    
            while (file >> uname >> hashedPass >> level) {
                if (uname == msg.getRecipient()) {
                    found = true;
                    break;
                }
            }
    
            file.close();
    
            if (!found) {
                cout << "Recipient not found. Message not sent.\n";
                return;
            }
    
            msg.saveToInbox();
            cout << "Message sent successfully to " << msg.getRecipient() << endl;
            logActivity("Message sent by " + sender->getUsername() + " to " + msg.getRecipient() + " [ID: " + to_string(msg.getID()) + "]");
        }
    }
    

    

    void Message::deleteMessage(int messageID, const string& username) {
        string filename = username + "_inbox.txt";
        ifstream inboxFile(filename);
        ofstream tempFile("temp_inbox.txt");
        string line;
        bool messageDeleted = false;
    
        if (inboxFile.is_open() && tempFile.is_open()) {
            while (getline(inboxFile, line)) {
                if (line.find("Message ID: " + to_string(messageID)) != string::npos) {
                    messageDeleted = true;
                    while (getline(inboxFile, line) && line != "----------------------------------") {
                    }
                    continue;
                }
                tempFile << line << endl;
            }
    
            inboxFile.close();
            tempFile.close();
    
            if (messageDeleted) {
                remove(filename.c_str());
                rename("temp_inbox.txt", filename.c_str());
                cout << "Message deleted successfully." << endl;
                logActivity("Message ID " + to_string(messageID) + " deleted by " + username);
            } else {
                remove("temp_inbox.txt");
                cout << "Message not found." << endl;
                logActivity("FAILED delete attempt for Message ID " + to_string(messageID) + " by " + username);
            }
        } else {
            cerr << "Unable to open inbox file!" << endl;
            logActivity("ERROR opening inbox file for delete attempt by " + username);
        }
    }
    
    
    void Message::listInboxMessages(const string& username) {
        string filename = username + "_inbox.txt";
        ifstream inboxFile(filename);
        string line;
    
        if (inboxFile.is_open()) {
            cout << "Listing messages for " << username << ":\n";
            while (getline(inboxFile, line)) {
                cout << line << endl;
            }
            inboxFile.close();
        } else {
            cerr << "Unable to open inbox file!" << endl;
        }
    }

    void Message::decryptPrivateMessageByID(int msgID, const string& recipient) {
        string filename = recipient + "_private_store.txt";
        ifstream file(filename);
        string line;
    
        if (!file.is_open()) {
            cout << "Could not open private store for " << recipient << endl;
            logActivity("FAILED to open private store for decryption attempt by " + recipient);
            return;
        }
    
        bool found = false;
    
        while (getline(file, line)) {
            if (line.find("ID:" + to_string(msgID)) != string::npos) {
                found = true;
                size_t pos = line.find("CONTENT:");
                if (pos != string::npos) {
                    string encryptedContent = line.substr(pos + 8);
    
                    for (char& ch : encryptedContent) {
                        if (ch >= 32 && ch <= 126) {
                            ch = (ch - 32 - 3 + 95) % 95 + 32;
                        }
                    }
    
                    cout << "Decrypted Message [" << msgID << "]: " << encryptedContent << endl;
                    logActivity("Message ID " + to_string(msgID) + " decrypted by " + recipient);
                } else {
                    cout << "CONTENT field not found in the message.\n";
                    logActivity("Malformed message format for ID " + to_string(msgID) + " during decryption by " + recipient);
                }
                break;
            }
        }
    
        file.close();
    
        if (!found) {
            cout << "Message ID not found or not PRIVATE." << endl;
            logActivity("Decryption failed: Message ID " + to_string(msgID) + " not found for " + recipient);
        }
    }
    
    

    void Message::readInbox(const User& currentUser) {
        string inboxFile = currentUser.getUsername() + "_inbox.txt";
        ifstream file(inboxFile);
        string line;
        string messageBlock[10];
        int lineCount = 0;
    
        if (!file.is_open()) {
            cout << "Inbox not found." << endl;
            logActivity("FAILED to open inbox for " + currentUser.getUsername() + " while attempting to read inbox.");
            return;
        }
    
        cout << "Reading inbox for: " << currentUser.getUsername() << "\n\n";
    
        while (getline(file, line)) {
            messageBlock[lineCount++] = line;
    
            if (line == "----------------------------------") {
                string typeLine = messageBlock[2]; 
                string contentLine = messageBlock[4];
    
                string type = typeLine.substr(6);  
                int clearance = currentUser.getClearanceLevel();
    
                if (type == "ALERT" && clearance < 3) {
                    cout << "Message skipped: You don't have clearance to read ALERT messages.\n";
                    logActivity("ALERT message skipped: " + currentUser.getUsername() + " does not have clearance to read ALERT.");
                } else {
                    for (int i = 0; i < lineCount; ++i)
                        cout << messageBlock[i] << endl;
                    cout << endl;
                    logActivity("Message read by " + currentUser.getUsername() + ": Message ID - " + messageBlock[0].substr(12)); // ID is at position 0
                }
    
                lineCount = 0;  
            }
        }
    
        file.close();
    }


    void Message::readLatestOTP(const string& recipientUsername) {
        string inboxFile = recipientUsername + "_inbox.txt";
        ifstream file(inboxFile);
        if (!file.is_open()) {
            cout << "Failed to open inbox for user: " << recipientUsername << "\n";
            return;
        }
    
        string line;
        string latestOTP = "";
        int latestID = -1;
    
        bool otpFound = false;
    
        while (getline(file, line)) {
            if (line == "----------------------------------") {
                string idLine, senderLine, recipientLine, typeLine, isReadLine, contentLine;
                string id, sender, type, content;
    
                getline(file, idLine);  
                getline(file, senderLine);  
                getline(file, recipientLine);  
                getline(file, typeLine);
                getline(file, isReadLine); 
                getline(file, contentLine);  
    
                size_t pos = idLine.find(":");
                if (pos != string::npos) id = idLine.substr(pos + 2); 
    
                pos = senderLine.find(":");
                if (pos != string::npos) sender = senderLine.substr(pos + 2); 
    
                pos = typeLine.find(":");
                if (pos != string::npos) type = typeLine.substr(pos + 2); 
    
                pos = contentLine.find(":");
                if (pos != string::npos) content = contentLine.substr(pos + 2);  
    
                int msgID = stoi(id);  
    
                if (sender == "System" && type == "INFO" && content.find("Your OTP is:") != string::npos) {
                    if (msgID > latestID) {
                        latestOTP = content;
                        latestID = msgID;
                        otpFound = true;
                    }
                }
    
                if (otpFound) {
                    break;
                }
            }
        }
    
        file.close();
    
        if (latestID == -1) {
            cout << "No OTP message found.\n";
        } else {
            cout << "\n[OTP MESSAGE]\n";
            cout << "From: System\n";
            cout << "Message: " << latestOTP << "\n";
        }
    }
            
            
            
            
            
            
            