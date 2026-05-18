#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;

struct Transaction
{
    string type;
    double amount;
    double balance;
};

struct Account {
    int accountNumber;
    string name;
    string contact;
    string pin;
    double balance;
    vector<Transaction> transactions;
    int failedAttempts;
    time_t lockoutTime;
};


vector<Account> accounts;
int nextAccountNumber = 1001;

bool verifyPin(int index)
{
    time_t now = time(0);

    if (accounts[index].failedAttempts >= 3)
    {
        double secondsLeft = difftime(accounts[index].lockoutTime + 1200, now);
        if (secondsLeft > 0)
        {
            cout << "Account locked! Try again in " << (int)(secondsLeft / 60) << " minutes." << endl;
            return false;
        }
        else
        {
            accounts[index].failedAttempts = 0;
        }
    }

    string enteredPin;
    cout << "Enter your PIN: ";
    cin >> enteredPin;

    if (enteredPin != accounts[index].pin)
    {
        accounts[index].failedAttempts++;
        if (accounts[index].failedAttempts >= 3)
        {
            accounts[index].lockoutTime = time(0);
            cout << "Too many wrong attempts! Account locked for 20 minutes." << endl;
        }
        else
        {
            cout << "Incorrect PIN! Attempts remaining: " << 3 - accounts[index].failedAttempts << endl;
        }
        return false;
    }

    accounts[index].failedAttempts = 0;
    return true;
}

void createAccount() {
    Account newAccount;
    newAccount.accountNumber = nextAccountNumber++;

    cout << "Enter your name: " << endl;
    cin.ignore(); // Clear the input buffer before using getline
    getline(cin, newAccount.name);

    cout << "Enter your contact information: " <<endl;
    cin >> newAccount.contact;

    string confirmPin;
    do
    {
    cout << "Set a 4-digit PIN: ";
        cin >> newAccount.pin;
        
        cout << "Confirm your PIN: ";
        cin >> confirmPin;
        if (newAccount.pin != confirmPin)
        {
            cout << "PINs do not match. Please try again." << endl;
        }
    } while (newAccount.pin != confirmPin);

    double initialDeposit;
    do
    {
        cout << "Enter initial deposit (minimum GHS 30): ";
        cin >> initialDeposit;
        if (initialDeposit < 30)
        {
            cout << "Minimum deposit is GHS 30. Please try again." << endl;
        }
    } while (initialDeposit < 30);

    newAccount.failedAttempts = 0;
    newAccount.lockoutTime = 0;

    newAccount.balance = initialDeposit;
    accounts.push_back(newAccount);

    cout << "Account created successfully! Your account number is: " << newAccount.accountNumber << endl;
}

void depositMoney() {
    int accNumber;
    cout << "Enter account number: ";
    cin >> accNumber;

    int index = -1;
    for (int i = 0; i < accounts.size(); i++){
        if (accounts[i].accountNumber == accNumber){
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Account not found!" << endl;
        return;
    }

    if (!verifyPin(index))
        return;


    double amount;
    cout << "Enter amount to deposit : " << endl;
    cin >> amount;

    if (amount <= 0) {
        cout << "Invalid amount!" << endl;
        return;
    }
    accounts[index].balance += amount;
    cout << "Deposit successful! New balance : GHS " << accounts[index].balance << endl;

    Transaction t;
    t.type = "Deposit";
    t.amount = amount;
    t.balance = accounts[index].balance;
    accounts[index].transactions.push_back(t);
}

void withdrawMoney(){
    int accNumber;

    cout << "Enter your Account Number :" << endl;
    cin >> accNumber;

    int index = -1;
    for (int i = 0; i < accounts.size(); i++)
    {
        if (accounts[i].accountNumber == accNumber)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        cout << "Account not found!" << endl;
        return;
    }

    if (!verifyPin(index))
        return;

    double amount;
    cout << "Enter amount to withdraw : " << endl;
    cin >> amount;

    if (accounts[index].balance - amount < 30)
    {
        cout << "Insufficient funds! Minimum balance of GHS 30 must remain." << endl;
        return;
    }

    accounts[index].balance -= amount;
    cout << "Withdrawal successful! New balance : GHS " << accounts[index].balance << endl;

    Transaction t;
    t.type = "Withdrawal";
    t.amount = amount;
    t.balance = accounts[index].balance;
    accounts[index].transactions.push_back(t);
}

void checkBalance() {
    int accNumber;

    cout << "Enter your Account Number :" << endl;
    cin >> accNumber;

    int index = -1;
    for (int i = 0; i < accounts.size(); i++)
    {
        if (accounts[i].accountNumber == accNumber)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        cout << "Account not found!" << endl;
        return;
    }

    if (!verifyPin(index))
        return;

    cout << "Account Holder: " << accounts[index].name << endl;
    cout << "Account Number: " << accounts[index].accountNumber << endl;
    cout << "Your current balance is : GHS " << accounts[index].balance << endl;

}

void viewTransactionHistory() {
    int accNumber;

    cout << "Enter your Account Number :" << endl;
    cin >> accNumber;

    int index = -1;
    for (int i = 0; i < accounts.size(); i++)
    {
        if (accounts[i].accountNumber == accNumber)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        cout << "Account not found!" << endl;
        return;
    }

    if (!verifyPin(index))
        return;

    if (accounts[index].transactions.empty())
    {
        cout << "No transaction history found." << endl;
        return;
    }

    cout << "Transaction History for Account Number: " << accounts[index].accountNumber << endl;
    for (const Transaction& t : accounts[index].transactions)
    {
        cout << t.type << ": GHS " << t.amount << " | Balance after transaction: GHS " << t.balance << endl;
    }
}

void fundTransfer() {
    int accNumber;

    cout << "Enter your Account Number :" << endl;
    cin >> accNumber;

    int index = -1;
    for (int i = 0; i < accounts.size(); i++)
    {
        if (accounts[i].accountNumber == accNumber)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        cout << "Account not found!" << endl;
        return;
    }

    if (!verifyPin(index))
        return;

    int receiverNumber;
    cout << "Enter receiver's account number: " << endl;
    cin >> receiverNumber;

    int receiverIndex = -1;
    for (int i = 0; i < accounts.size(); i++)
    {
        if (accounts[i].accountNumber == receiverNumber)
        {
            receiverIndex = i;
            break;
        }
    }

    if (receiverIndex == -1)
    {
        cout << "Receiver account not found!" << endl;
        return;
    }

    double amount;
    cout << "Enter amount to transfer: " << endl;
    cin >> amount;

    if (accounts[index].balance - amount < 30)
    {
        cout << "Insufficient funds! Minimum balance of GHS 30 must remain." << endl;
        return;
    }

    accounts[index].balance -= amount;
    accounts[receiverIndex].balance += amount;

    Transaction t1;
    t1.type = "Transfer Out";
    t1.amount = amount;
    t1.balance = accounts[index].balance;
    accounts[index].transactions.push_back(t1);

    Transaction t2;
    t2.type = "Transfer In";
    t2.amount = amount;
    t2.balance = accounts[receiverIndex].balance;
    accounts[receiverIndex].transactions.push_back(t2);

    cout << "Transfer successful! Your new balance: GHS " << accounts[index].balance << endl;
}

void saveAccounts()
{
    ofstream file("accounts.txt");
    for (int i = 0; i < accounts.size(); i++)
    {
        file << accounts[i].accountNumber << "\n";
        file << accounts[i].name << "\n";
        file << accounts[i].contact << "\n";
        file << accounts[i].pin << "\n";
        file << accounts[i].balance << "\n";
        file << accounts[i].failedAttempts << "\n";
        file << accounts[i].lockoutTime << "\n";
    }
    file.close();
}

void loadAccounts()
{
    ifstream file("accounts.txt");
    if (!file)
        return;

    Account acc;
    while (file >> acc.accountNumber)
    {
        file.ignore();
        getline(file, acc.name);
        getline(file, acc.contact);
        getline(file, acc.pin);
        file >> acc.balance;
        file >> acc.failedAttempts;
        file >> acc.lockoutTime;
        file.ignore();
        accounts.push_back(acc);
        if (acc.accountNumber >= nextAccountNumber)
        {
            nextAccountNumber = acc.accountNumber + 1;
        }
    }
    file.close();
}

int main(){
    int menu;
    loadAccounts();

    do{

    cout << "" << endl;

    cout << "  Welcome to CodeAlpha Banking System"<<endl;

    cout << ""<<endl;
    cout << "  Please select an option:"<<endl;

    cout << "" << endl;

    cout << "  1. Create Account"<<endl;
    cout << "  2. Deposit Money"<<endl;
    cout << "  3. Withdraw Money"<<endl;
    cout << "  4. Check Balance"<<endl;
    cout << "  5. Transaction History"<<endl;
    cout << "  6. Fund Transfer"<<endl;
    cout << "  7. Exit"<<endl;

    cin >> menu;

    switch(menu){
        case 1:
            createAccount();
            break;
        case 2:
            depositMoney();
            break;
        case 3:
            withdrawMoney();
            break;
        case 4:
            checkBalance();
            break;
        case 5:
            viewTransactionHistory();
            break;
        case 6:
            fundTransfer();
            break;
            
        case 7:
            saveAccounts();
            cout << "Thank you for using CodeAlpha Banking System. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
    }  

} while (menu !=7);

        return 0;
}