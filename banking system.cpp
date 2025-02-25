#include <iostream>
#include <vector>
#include <string>

using namespace std;
class Customer {
public:
    string name;
    int customerID;

    Customer(string n, int id) : name(n), customerID(id) {}
};
class Account {
protected:
    int accountNumber;
    double balance;
    Customer* customer;
    vector<string> transactionHistory;

public:
    Account(int accNo, Customer* cust) : accountNumber(accNo), balance(0.0), customer(cust) {}

    void deposit(double amount) {
        balance += amount;
        transactionHistory.push_back("Deposited $" + to_string(amount));
        cout << "Deposited $" << amount << " successfully!\n";
    }

    virtual bool withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient balance!\n";
            return false;
        }
        balance -= amount;
        transactionHistory.push_back("Withdrew $" + to_string(amount));
        cout << "Withdrawal of $" << amount << " successful!\n";
        return true;
    }

    void displayBalance() {
        cout << "Account Number: " << accountNumber << " | Balance: $" << balance << endl;
    }

    void showTransactionHistory() {
        cout << "Transaction History for Account " << accountNumber << ":\n";
        for (const auto& txn : transactionHistory) {
            cout << "- " << txn << endl;
        }
    }
};
class SavingsAccount : public Account {
public:
    SavingsAccount(int accNo, Customer* cust) : Account(accNo, cust) {}

    bool withdraw(double amount) override {
        if (balance - amount < 100) {
            cout << "Minimum balance of $100 must be maintained!\n";
            return false;
        }
        return Account::withdraw(amount);
    }
};

class Transaction {
public:
    static void transfer(Account& from, Account& to, double amount) {
        if (from.withdraw(amount)) {
            to.deposit(amount);
            cout << "Transfer successful!\n";
        } else {
            cout << "Transfer failed!\n";
        }
    }
};

class Bank {
private:
    vector<Customer> customers;
    vector<Account*> accounts;

public:
    void createCustomer(string name, int id) {
        customers.push_back(Customer(name, id));
        cout << "Customer " << name << " created successfully!\n";
    }

    Account* createAccount(int accNo, int custID, bool isSavings = false) {
        for (Customer& cust : customers) {
            if (cust.customerID == custID) {
                Account* acc = isSavings ? new SavingsAccount(accNo, &cust) : new Account(accNo, &cust);
                accounts.push_back(acc);
                cout << "Account created successfully!\n";
                return acc;
            }
        }
        cout << "Customer not found!\n";
        return nullptr;
    }

    void displayAccounts() {
        for (auto acc : accounts) {
            acc->displayBalance();
        }
    }
};

// Main Function
int main() {
    Bank bank;

    // Creating Customers
    bank.createCustomer("Alice", 101);
    bank.createCustomer("Bob", 102);

    // Creating Accounts
    Account* acc1 = bank.createAccount(1001, 101, true);  
    Account* acc2 = bank.createAccount(1002, 102);

    // Transactions
    if (acc1) acc1->deposit(500);
    if (acc2) acc2->deposit(300);
    if (acc1) acc1->withdraw(200);
    if (acc2 && acc1) Transaction::transfer(*acc2, *acc1, 100);

    // Display Account Balances
    bank.displayAccounts();

    // Show Transaction History
    if (acc1) acc1->showTransactionHistory();
    if (acc2) acc2->showTransactionHistory();

    return 0;
}
