#include <iostream>
#include <string>
using namespace std;

class PaymentGateway {
public:
    virtual void processPayment(double amount) = 0;
    virtual ~PaymentGateway() {}
};

class Invoice {
public:
    virtual void generateInvoice() = 0;
    virtual ~Invoice() {}
};

// ========== India Implementations ==========
class RazorpayGateway : public PaymentGateway {
public:
    void processPayment(double amount) override {
        cout << "Processing INR payment via Razorpay: " << amount << endl;
    }
};

class PayUGateway : public PaymentGateway {
public:
    void processPayment(double amount) override {
        cout << "Processing INR payment via PayU: " << amount << endl;
    }
};

class GSTInvoice : public Invoice {
public:
    void generateInvoice() override {
        cout << "Generating GST Invoice for India." << endl;
    }
};

// ========== US Implementations ==========
class PayPalGateway : public PaymentGateway {
public:
    void processPayment(double amount) override {
        cout << "Processing USD payment via PayPal: " << amount << endl;
    }
};

class StripeGateway : public PaymentGateway {
public:
    void processPayment(double amount) override {
        cout << "Processing USD payment via Stripe: " << amount << endl;
    }
};

class USInvoice : public Invoice {
public:
    void generateInvoice() override {
        cout << "Generating Invoice as per US norms." << endl;
    }
};

// ========== Abstract Factory ==========
class RegionFactory {
public:
    virtual PaymentGateway* createPaymentGateway(string gatewayType) = 0;
    virtual Invoice* createInvoice() = 0;
    virtual ~RegionFactory() {}
};

// ========== Concrete Factories ==========
class IndiaFactory : public RegionFactory {
public:
    PaymentGateway* createPaymentGateway(string gatewayType) override {
        if (gatewayType == "razorpay") {
            return new RazorpayGateway();
        } else if (gatewayType == "payu") {
            return new PayUGateway();
        }
        throw invalid_argument("Unsupported gateway for India: " + gatewayType);
    }

    Invoice* createInvoice() override {
        return new GSTInvoice();
    }
};

class USFactory : public RegionFactory {
public:
    PaymentGateway* createPaymentGateway(string gatewayType) override {
        if (gatewayType == "paypal") {
            return new PayPalGateway();
        } else if (gatewayType == "stripe") {
            return new StripeGateway();
        }
        throw invalid_argument("Unsupported gateway for US: " + gatewayType);
    }

    Invoice* createInvoice() override {
        return new USInvoice();
    }
};

// ========== Checkout Service ==========
class CheckoutService {
private:
    PaymentGateway* paymentGateway;
    Invoice* invoice;

public:
    CheckoutService(RegionFactory* factory, string gatewayType) {
        paymentGateway = factory->createPaymentGateway(gatewayType);
        invoice = factory->createInvoice();
    }

    void completeOrder(double amount) {
        paymentGateway->processPayment(amount);
        invoice->generateInvoice();
    }

    ~CheckoutService() {
        delete paymentGateway;
        delete invoice;
    }
};

// ========== Main Method ==========
int main() {
    // Using Razorpay in India
    CheckoutService indiaCheckout(new IndiaFactory(), "razorpay");
    indiaCheckout.completeOrder(1999.0);

    cout << "---" << endl;

    // Using PayPal in US
    CheckoutService usCheckout(new USFactory(), "paypal");
    usCheckout.completeOrder(49.99);

    return 0;
}
