#include <iostream>
#include <memory>
#include <string>

using namespace std;

// Target Interface:
// Standard interface expected by the CheckoutService
class PaymentGateway {
public:
    // Virtual destructor for safe cleanup
    virtual ~PaymentGateway() = default;

    // Client expects pay(orderId, amount)
    virtual void pay(const string& orderId, double amount) = 0;
};

// Concrete implementation for PayU
class PayUGateway : public PaymentGateway {
public:
    void pay(const string& orderId, double amount) override {
        cout << "Paid Rs. " << amount << " using PayU for order: " << orderId << "\n";
    }
};

// Adaptee:
// Existing class with incompatible method name and parameter naming
class RazorpayAPI {
public:
    void makePayment(const string& invoiceId, double amountInRupees) {
        cout << "Paid Rs. " << amountInRupees << " using Razorpay for invoice: " << invoiceId << "\n";
    }
};

// Adapter Class:
// Allows RazorpayAPI to be used where PaymentGateway is expected
class RazorpayAdapter : public PaymentGateway {
private:
    shared_ptr<RazorpayAPI> razorpayAPI;

public:
    // Construct adapter with dependency
    RazorpayAdapter() {
        razorpayAPI = make_shared<RazorpayAPI>();
    }

    // Translate pay() call to RazorpayAPI::makePayment()
    void pay(const string& orderId, double amount) override {
        // Mapping orderId to invoiceId (conceptual mapping)
        razorpayAPI->makePayment(orderId, amount);
    }
};

// Client Class:
// Uses PaymentGateway interface to process payments
class CheckoutService {
private:
    shared_ptr<PaymentGateway> paymentGateway;

public:
    // Constructor injection
    CheckoutService(shared_ptr<PaymentGateway> gateway) : paymentGateway(gateway) {}

    // Business logic
    void checkout(const string& orderId, double amount) {
        paymentGateway->pay(orderId, amount);
    }
};

int main() {
    // Using Razorpay adapter to process payment
    shared_ptr<PaymentGateway> gateway = make_shared<RazorpayAdapter>();

    // CheckoutService stays unchanged
    CheckoutService checkoutService(gateway);

    // Client uses the standard interface
    checkoutService.checkout("12", 1780);

    return 0;
}
