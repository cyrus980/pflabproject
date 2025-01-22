// -----------------------------  Importing Libraries ---------------------------------------------

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;


// ------------------------------ Account Structure ----------------------------------------------------

struct Account {
    string name, acc_number, acc_pin;
    double balance = 0;
};


// ----------------------------- Accounts Array ----------------------------------------------------------

Account accounts[100];


// ---------------------------- Active Accounts (Current number of accounts in *accounts* array) -------

int active_accounts = 0;

// -----------------------------  Function to save account data to a CSV file ---------------------------

void saveAccountsToCSV() {
    ofstream file("accounts.csv");

    if (file) {
	    // Write the account details to the file
	    for (int i = 0; i < active_accounts; i++) {
	        file << accounts[i].name << ","
	             << accounts[i].acc_number << ","
	             << accounts[i].acc_pin << ","
	             << accounts[i].balance << endl;
	    }
    }
    else {
		cout << "Error opening the file for writing!" << endl;
	}	


    file.close();
}



// ------------ Function to load account data from a CSV file and update the accounts array --------------------

void loadAccountsFromCSV() {
    ifstream file("accounts.csv");

    if (file) {
        string line;
    	active_accounts = 0;  // Reset the count of active accounts

    	// Read data from the file
	    while (getline(file, line) && active_accounts < 100) {
	        stringstream ss(line);
	        string name, acc_number, acc_pin, balance_str;
	        double balance;
	
	        // Extract data from the line
	        getline(ss, name, ',');
	        getline(ss, acc_number, ',');
	        getline(ss, acc_pin, ',');
	        getline(ss, balance_str, ',');
	
	        balance = stod(balance_str);  // Convert balance to double
	
	        // Store the account data into the accounts array
	        accounts[active_accounts].name = name;
	        accounts[active_accounts].acc_number = acc_number;
	        accounts[active_accounts].acc_pin = acc_pin;
	        accounts[active_accounts].balance = balance;
	
	        active_accounts++; // Increase the active accounts count
	    }
	}

    file.close();
}


// ----------------------------- Whether Account Exist ? --------------------------------------------------

int findAccount(string account_number) {
	
    for (int i = 0; i < active_accounts; i++) {
        if (accounts[i].acc_number == account_number) {
            return i; // Returning account index
        }
    }
    return -1; // Account does not exist after checking all accounts
}


// --------------------------- Login Function ---------------------------------------------------------

int login(){
	string entered_account_number, entered_pin;
	
	cout << "\nEnter account number : ";
	cin >> entered_account_number;
	
	cout << "Enter pin : ";
	cin >> entered_pin;
	
	
	int account_index = findAccount(entered_account_number);
	
	if(account_index != -1){
		
		if(accounts[account_index].acc_pin == entered_pin){
			cout << "\nSuccessfully Logged in " << accounts[account_index].name << "'s account!";
			return account_index;
		}
		else {
			cout << "\nWoops! Invalid PIN."; // Exception handeling ( In case of Invalid PIN )
		}
		
	}
	else {
		cout << "\nWoops! Account not found."; 	// Exception handeling ( if account not found *accountindex* will be -1 )
	}
	
	return -1;
}

// --------------------------- Create Account Function ------------------------------------------------

void createAccount(){
	long long int deposited_amount;
	
	
	cout << "\nEnter name : ";
	cin >> accounts[active_accounts].name;
	cout << "\nEnter account number : ";
	cin >> accounts[active_accounts].acc_number;
	cout << "\nSet Pin : ";
	cin >> accounts[active_accounts].acc_pin;
	
	// Taking initial deposite from user 
	while(true) {
		cout << "\nInitial Deposite : ";
		cin >> deposited_amount;
		
		if(deposited_amount < 0){
			cout << "\n>> Woops! Initial Deposite must be greater than 0.\n";	 // Exception handeling (initial amount >0)
		}else{
			accounts[active_accounts].balance = deposited_amount;
			break;
		}	
	}
	
	
	cout << "\n>> Account Created Successfully!";
	active_accounts++;
	saveAccountsToCSV();
}


// --------------------------- Deposite Money Function ------------------------------------------------

void depositMoney(){
	int logged_in_account_index = login();
	
	if(logged_in_account_index != -1){
		int entered_deposite_money;
		
		cout << "\n\nEnter money you want to deposite : ";
		cin >> entered_deposite_money;
		
		if(entered_deposite_money > 0){
			cout << "\n>> Operation Successfull ! : " << entered_deposite_money << " deposited.";
			accounts[logged_in_account_index].balance += entered_deposite_money;
			
			saveAccountsToCSV();
		}else{
			cout << "\n>> Woops! deposited money must be greater than 0"; // Exception handeling (money must be greater than 0)
		}
	}
}

// --------------------------- Withdraw Money Function ------------------------------------------------

void withdrawMoney(){
	int logged_in_account_index = login();
	
	if(logged_in_account_index != -1){
		int entered_withdraw_money;
		
		cout << "\n\nEnter money you want to withdraw : ";
		cin >> entered_withdraw_money;
		
		if(accounts[logged_in_account_index].balance >= entered_withdraw_money){
			cout << "\n>> Transaction Successfull ! : " << entered_withdraw_money << " withdrawn.";
			accounts[logged_in_account_index].balance -= entered_withdraw_money;
			
			saveAccountsToCSV();	
		}
		else {
			cout << "\n>> Operation Unsuccessfull! : Balance is insufficient.";  // Exception handeling (In case if balance is insufficent)
		}
	}
}

// --------------------------- Check Balance Function --------------------------------------------------

void checkBalance(){
	int logged_in_account_index = login();
	
	if(logged_in_account_index != -1){
		cout << "\n\n>> Current balance : " << accounts[logged_in_account_index].balance;
	}
}

// --------------------------- Transfer Money Function --------------------------------------------------

void transferMoney() {
    // Flag to check if account is found
    int logged_in_account_index = login();
    
    if(logged_in_account_index != -1){
    	
    	bool account_found = false;
    	
		string transfer_account_number;
		cout << "\n\nSend to (Enter account number) : ";
		cin >> transfer_account_number;
		
	    for (int i = 0; i < active_accounts; i++) {
	        if (accounts[i].acc_number == transfer_account_number) {
	            account_found = true;
	            cout << "\n>> Information : Account holder name is " << accounts[i].name;
	            
	            int deposit_amount;
	            cout << "\n\nEnter amount you want to send : ";
	            cin >> deposit_amount;
	            
	            if (deposit_amount > 0) {
	            	if(accounts[logged_in_account_index].balance >= deposit_amount){
		                accounts[i].balance += deposit_amount;
		                
		                accounts[logged_in_account_index].balance -= deposit_amount;
		                
		                cout << "\n>> Transaction Successful! " << deposit_amount <<" transfered.";
		                saveAccountsToCSV();
					}
					else{
						cout << "\n>> Operation Unsuccessfull! : Balance is insufficient.";  // Exception handeling (In case if balance is insufficent)
					}
	            }
				else {
	                cout << "\n>> Woops! Deposit amount must be greater than zero.\n"; // Exception  handeling
	            }
	            
	            break;
	        }
	    }
		
	    if (!account_found) {
	        cout << "\nWoops! Account not found.";
	    }   
	}
}


// --------------------------- Delete Account Function --------------------------------------------------

void deleteAccount() {
    cout << "\nEnter details of the account you want to delete.";
    int logged_in_account_index = login();

    if (logged_in_account_index != -1) {
        char want_to_del = 'n';
        cout << "\nDo you want to delete the account (y/n)? : ";
        cin >> want_to_del;

        if (want_to_del == 'y' || want_to_del == 'Y') {
            // Fetch the account number to display
            string account_number = accounts[logged_in_account_index].acc_number;

            // Shift elements to the left starting from the index of the account to delete
            for (int i = logged_in_account_index; i < active_accounts - 1; i++) {
                accounts[i] = accounts[i + 1];
            }

            active_accounts--; // Reduce the number of active accounts

            cout << "\n>> Operation successful! Account " << account_number << " deleted successfully.";
            saveAccountsToCSV();
        }
        else {
        	cout << "\n>> Operation Unsuccessful!";
		}
    }
}

// --------------------------- Main Function ----------------------------------------------------------


int main() {
	
	loadAccountsFromCSV();
	
    int choice;

    do {
        // Main menu
        cout << "\n\n===================================" << endl;
        cout << "        	Menu      " << endl;
        cout << "===================================" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. Deposit Money" << endl;
        cout << "3. Withdraw Money" << endl;
        cout << "4. Check Balance" << endl;
        cout << "5. Transfer Money" << endl;
        cout << "6. Delete Account" << endl;
        cout << "7. Exit Program" << endl;
        cout << "===================================" << endl;
        cout << "Enter your choice (1-7): ";
        cin >> choice;

        // Validate input
        if (choice < 1 || choice > 7) {
            cout << "\nInvalid choice! Please enter a number between 1 and 7.\n";
            continue;
        }

        // Menu handling
        switch (choice) {
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
                transferMoney();
                break;
            case 6:
                deleteAccount();
                break;
            case 7:
                break;
            default:
                cout << "Unexpected error occurred!" << endl;
        }

    } while (choice != 7);

    return 0;
}
