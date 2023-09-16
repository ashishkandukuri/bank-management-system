#include<iostream>
#include<iomanip>
#include<fstream>
#include<cctype>

using namespace std;

class bankAccount{
    long long accountNumber;
    char name[50];
    char type;
    int moneyDeposit;

public: 
    void report() const;
    long long returnAccountNumber() const{
        return accountNumber;
    }
    void deposit(int x){
        moneyDeposit+=x;
    }
    void withDraw(int x){
        moneyDeposit-=x;
    }
    int returnDeposit() const{
        return moneyDeposit;
    }
    void modify();
    void createAccount();
    void displayAccount();
};

void bankAccount::report() const{
    cout<<accountNumber<<setw(10)<<name<<setw(8)<<type<<setw(11)<<moneyDeposit<<endl;
}

void bankAccount::createAccount(){
    system("cls");
    cout<<"\t Enter the account number: ";
    cin>>accountNumber;
    cout<<"\t Enter the name of the account holder: ";
    cin.ignore();
    cin.getline(name,50);
    cout<<"\t Enter the type of the account (c/s): ";
    cin>>type;
    type = toupper(type);
    cout<<"\t Enter the initial amount (>=500 for saving and >=1000 for current): ";
    cin>>moneyDeposit;
    if((type=='C'&&moneyDeposit<1000)||(type=='S'&&moneyDeposit<500)){
        cout<<"\t Amount entered is not sufficient for the type of account chosen"<<endl;
        cout<<"\t Enter valid amount again: ";
        cin>>moneyDeposit;
    }
    else{
        cout<<endl;
        cout<<"\t Account Created....  :)";
        cout<<endl;
    }
}

void bankAccount::displayAccount(){
    cout<<"\t Bank Account Number: "<<accountNumber<<endl;
    cout<<"\t Account Holder Name: "<<name<<endl;
    cout<<"\t Type Of Account: "<<type<<endl;
    cout<<"\t Balance Amount: "<<moneyDeposit<<endl;
}

void bankAccount::modify(){
    cout<<"\t Account Number: "<<accountNumber<<endl;
    cout<<"\t Modify Account Holder Name: ";
    cin.ignore();
    cin.getline(name,50);
    cout<<"\t Modify Type Of Account: ";
    cin>>type;
    type = toupper(type);
}

void writeAccount(){
    bankAccount ac;
    ofstream outFile;
    outFile.open("account.dat",ios::binary|ios::app);
    ac.createAccount();
    outFile.write(reinterpret_cast<char *> (&ac), sizeof(bankAccount));
    outFile.close(); 
}

void deleteAccount(long long n){
    bool flag = false;
    bankAccount ac;
    ifstream inFile;
    ofstream outFile;
    inFile.open("account.dat",ios::binary);
    if(!inFile){
        cout<<"File could not be open || press any key...";
        return;
    }
    outFile.open("temp.dat",ios::binary);
    inFile.seekg(0,ios::beg);

    while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(bankAccount))){
        if(ac.returnAccountNumber()==n) flag = true;
        if(ac.returnAccountNumber()!=n){
            outFile.write(reinterpret_cast<char *> (&ac),sizeof(bankAccount));
        }
    }
    inFile.close();
    outFile.close();

    remove("account.dat");
    rename("temp.dat","account.dat");
    (flag)?cout<<"\t Account/Record Deleted... ":cout<<"\t Account number does not exist... ";
}

void displayDetails(long long n){
    bool flag = false;
    bankAccount ac;
    ifstream inFile;
    inFile.open("account.dat",ios::binary);
    if(!inFile){
        cout<<"File could not be open || press any key...";
        return;
    }
    cout<<"\t Bank Account Details: "<<endl;
    while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(bankAccount))){
        if(ac.returnAccountNumber()==n){
            ac.displayAccount();
            flag = true;
        }
    }
    inFile.close();
    if(!flag) cout<<"\t Account number does not exist... "<<endl;
}

void displayAllDetails(){
    system("cls");
    bankAccount ac;
    ifstream inFile;
    inFile.open("account.dat",ios::binary);
    if(!inFile){
        cout<<"File could not be open!! ";
        return;
    }
    cout<<"\t Bank Account Holders List"<<endl;
    cout<<"=========================================="<<endl;
    cout<<"A/C no.         Name      Type     Balance"<<endl;
    cout<<"=========================================="<<endl;

    while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(bankAccount))){
        ac.report();
    }
    inFile.close();
}

void moneyDepositWithdraw(long long n,int id){
    int amount;
    bool found = false;
    bankAccount ac;
    fstream File;
    File.open("account.dat",ios::binary|ios::in|ios::out);
    if(!File){
        cout<<"File could not be open... ";
        return;
    }
    while(!File.eof()&&found==false){
        File.read(reinterpret_cast<char *> (&ac),sizeof(bankAccount));
        if(ac.returnAccountNumber()==n){
            ac.displayAccount();
            if(id==1){
                cout<<"\t Enter the amount to be deposited: ";
                cin>>amount;
                ac.deposit(amount);
            }
            if(id==2){
                cout<<"\t Enter the amount to be withdraw: ";
                cin>>amount;
                int balance = ac.returnDeposit()-amount;
                if(balance<0){
                    cout<<"\t Insufficient Balance "<<endl;
                }
                else ac.withDraw(amount);
            }
            int pos = (-1)*static_cast<int>(sizeof(bankAccount));
            File.seekp(pos,ios::cur);
            File.write(reinterpret_cast<char *> (&ac),sizeof(bankAccount));
            cout<<"\t Account/Record updated... "<<endl;
            found = true;
        }  
    }
    File.close();
    if(!found) cout<<"\t Account not found... "<<endl;
}

void updateAccount(long long n){
    bool found = false;
    bankAccount ac;
    fstream File;
    File.open("account.dat",ios::binary|ios::in|ios::out);
    if(!File){
        cout<<"\t File could not be open... ";
        return;
    }
    while(!File.eof()&&found==false){
        File.read(reinterpret_cast<char *> (&ac), sizeof(bankAccount));
        if(ac.returnAccountNumber()==n){
            ac.displayAccount();
            cout<<"\t Enter the new details of the account"<<endl;
            ac.modify();
            int pos = (-1)*static_cast<int>(sizeof(bankAccount));
            File.seekp(pos,ios::cur);
            File.write(reinterpret_cast<char *> (&ac),sizeof(bankAccount));
            cout<<"\t Account/Record updated... "<<endl;
            found = true;
        }
    }
    File.close();
    if(!found) cout<<"\t Account not found... "<<endl;
}

int main(){
    long long num;
    char ch;

    do{
        cout<<"\t\t -----------------------------------------"<<endl;
        cout<<"\t\t | Welcome To The Bank Management System | "<<endl;
        cout<<"\t\t -----------------------------------------"<<endl;

        cout<<endl;
        cout<<"\t\t ---Main Menu--- "<<endl;
        cout<<endl;
        cout<<"\t\t 1. Create Account"<<endl;
        cout<<"\t\t 2. Deposit Money"<<endl;
        cout<<"\t\t 3. Withdraw Money"<<endl;
        cout<<"\t\t 4. Balance Enquiry"<<endl;
        cout<<"\t\t 5. All Account Holder List"<<endl;
        cout<<"\t\t 6. Close Account"<<endl;
        cout<<"\t\t 7. Modify Account"<<endl;
        cout<<"\t\t 8. Exit"<<endl;
        cout<<endl;

        cout<<"\t Enter Your Choice: ";
        cin>>ch;

        switch(ch){
            case '1':
                writeAccount();
                break;
            case '2':
                system("cls");
                cout<<"\t Enter Your Account Number: ";
                cin>>num;
                moneyDepositWithdraw(num,1);
                break;
            case '3':
                system("cls");
                cout<<"\t Enter Your Account Number: ";
                cin>>num;
                moneyDepositWithdraw(num,2);
                break;
            case '4':
                system("cls");
                cout<<"\t Enter Your Account Number: ";
                cin>>num;
                displayDetails(num);
                break;
            case '5':
                system("cls");
                displayAllDetails();
                break;
            case '6':
                system("cls");
                cout<<"\t Enter Your Account Number: ";
                cin>>num;
                deleteAccount(num);
                break;
            case '7':
                system("cls");
                cout<<"\t Enter Your Account Number: ";
                cin>>num;
                updateAccount(num);
                break;
            case '8':
                cout<<endl;
                cout<<"\t Thanks For Using The Bank Management Sysytem!!! "<<endl;
                break;
        }
        cin.ignore();
        cin.get();
    }while(ch!='8');
    return 0;
}


