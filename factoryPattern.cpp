#include <iostream>
using namespace std;
class Notification {
public:
    virtual void send(string message) = 0;
    virtual ~Notification() {}
};
class EmailNotification : public Notification {
public:
    void send(string message) override {
        cout << "Sending EMAIL: " << message << endl;
    }
};

class SMSNotification : public Notification {
public:
    void send(string message) override {
        cout << "Sending SMS: " << message << endl;
    }
};

class PushNotification : public Notification {
public:
    void send(string message) override {
        cout << "Sending PUSH notification: " << message << endl;
    }
};

class NotificationFactory {
public:
    static Notification* createNotification(string type) {

        if (type == "email")
            return new EmailNotification();

        else if (type == "sms")
            return new SMSNotification();

        else if (type == "push")
            return new PushNotification();

        else {
            cout << "Invalid notification type\n";
            return nullptr;
        }
    }
};

int main() {

    Notification* n1 = NotificationFactory::createNotification("email");
    if(n1) n1->send("Welcome to our platform!");

    Notification* n2 = NotificationFactory::createNotification("sms");
    if(n2) n2->send("Your OTP is 4821");

    Notification* n3 = NotificationFactory::createNotification("push");
    if(n3) n3->send("You have a new message");

    delete n1;
    delete n2;
    delete n3;

    return 0;
}