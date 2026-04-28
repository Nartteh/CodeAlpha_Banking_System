#include <iostream>
using namespace std;

int main(){
    int menu;

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

} while (menu !=7);

        return 0;
}