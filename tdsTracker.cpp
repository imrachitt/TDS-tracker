#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

class Payee {
public:
    string name;
    string pan;
    int category; // 1 for individual, 2 for company

    Payee(string n, string p, int c) {
        name = n;
        pan = p;
        category = c;
    }
};

class Payer {
public:
    string name;
    string pan;
    string tan;

    Payer(string n, string p, string t) {
        name = n;
        pan = p;
        tan = t;
    }
};

class Payment {
private:
    Payer payer;
    Payee payee;
    int type;
    double amount;
    double tds;
    string section;
    string date;

public:
    Payment(Payer py, Payee pe, int t, double amt, string d)
        : payer(py), payee(pe), type(t), amount(amt), date(d) {
        calculateTDS();
    }

    void calculateTDS() {
        double rate = 0.0;
        if (type == 1) {
            section = "194J";
            rate = (amount < 50000) ? 0.0 : 0.10;
        } else if (type == 2) {
            section = "194I";
            rate = (amount < 50000) ? 0.0 : 0.10;
        } else if (type == 3) {
            section = "194C";
            rate = (payee.category == 1) ? 0.01 : 0.02;
        } else if (type == 4) {
            section = "192";
            if (amount <= 35000) rate = 0.0;
            else if (amount <= 70000) rate = 0.05;
            else if (amount <= 100000) rate = 0.10;
            else rate = 0.20;
        } else {
            section = "Unknown";
            rate = 0.0;
        }

        tds = amount * rate;
    }

    void showCertificate() {
        cout << fixed << setprecision(2);
        cout << "\n--- TDS CERTIFICATE ---\n";
        cout << "Payee Name   : " << payee.name << endl;
        cout << "Payee PAN    : " << payee.pan << endl;
        cout << "Payment Type : ";
        switch(type){
            case 1: cout << "Professional\n"; break;
            case 2: cout << "Rent\n"; break;
            case 3: cout << "Contractor\n"; break;
            case 4: cout << "Salary\n"; break;
            default: cout << "Unknown\n";
        } 
        cout << "Section      : " << section << endl;
        cout << "Gross Amount : " << amount << " INR" << endl;
        cout << "TDS Deducted : " << tds << " INR" << endl;
        cout << "Net Amount   : " << amount - tds << " INR" << endl;
        cout << "Payment Date : " << date << endl;
        cout << "------------------------\n";
    }

    void saveToFile() {
        ofstream file("tds_records.txt", ios::app);
        file << "Payee Name: " << payee.name << "\n";
        file << "Payee PAN: " << payee.pan << "\n";
        file << "Payment Type: ";
        switch(type){
            case 1: file << "Professional\n"; break;
            case 2: file << "Rent\n"; break;
            case 3: file << "Contractor\n"; break;
            case 4: file << "Salary\n"; break;
            default: file << "Unknown\n";
        } 
        file << "Section: " << section << "\n";
        file << fixed << setprecision(2);
        file << "Gross Amount: " << amount << " INR\n";
        file << "TDS Deducted: " << tds << " INR\n";
        file << "Net Amount: " << amount - tds << " INR\n";
        file << "Payment Date: " << date << "\n";
        file << "-------------------------\n";
        file.close();
        cout << "Certificate saved to file successfully.\n";
    }
};

int main() {
    vector<Payee> payees;
    vector<Payer> payers;
    int choice;

    while (true) {
        cout << "\n======= TDS MENU =======\n";
        cout << "1. Add Payee\n";
        cout << "2. Add Payer\n";
        cout << "3. Make Payment & Generate TDS\n";
        cout << "4. View Saved TDS Records (From File)\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string name, pan;
            int category;
            cout << "Enter Payee Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter PAN: ";
            cin >> pan;
            cout << "Enter Category (1 for Individual, 2 for Company): ";
            cin >> category;
            payees.push_back(Payee(name, pan, category));
            cout << "Payee added!\n";
        }
        else if (choice == 2) {
            string name, pan, tan;
            cout << "Enter Payer Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter PAN: ";
            cin >> pan;
            cout << "Enter TAN: ";
            cin >> tan;
            payers.push_back(Payer(name, pan, tan));
            cout << "Payer added!\n";
        }
        else if (choice == 3) {
            if (payees.empty() || payers.empty()) {
                cout << "Add both a Payee and a Payer first.\n";
                continue;
            }

            cout << "Select Payer:\n";
            for (size_t i = 0; i < payers.size(); ++i)
                cout << i + 1 << ". " << payers[i].name << "\n";
            int payerIndex;
            cin >> payerIndex;

            if (payerIndex < 1 || payerIndex > payers.size()) {
                cout << "Invalid Payer selection.\n";
                continue;
            }

            cout << "Select Payee:\n";
            for (size_t i = 0; i < payees.size(); ++i)
                cout << i + 1 << ". " << payees[i].name << " (" << (payees[i].category == 1 ? "Individual" : "Company") << ")\n";
            int payeeIndex;
            cin >> payeeIndex;

            if (payeeIndex < 1 || payeeIndex > payees.size()) {
                cout << "Invalid Payee selection.\n";
                continue;
            }

            int type;
            string date;
            double amount;

            cout << "Enter Payment Type:\n1. Professional\n2. Rent\n3. Contractor\n4. Salary\nEnter your choice: ";
            cin >> type;
            cout << "Enter Amount (INR): ";
            cin >> amount;
            cout << "Enter Payment Date (YYYY-MM-DD): ";
            cin >> date;

            Payment p(payers[payerIndex - 1], payees[payeeIndex - 1], type, amount, date);
            p.showCertificate();
            p.saveToFile();
        }
        else if (choice == 4) {
            ifstream file("tds_records.txt");
            if (!file.is_open()) {
                cout << "No records found.\n";
            } else {
                cout << "\n=== SAVED TDS CERTIFICATES ===\n";
                string line;
                while (getline(file, line)) {
                    cout << line << endl;
                }
                file.close();
            }
        }
        else if (choice == 5) {
            cout << "Exiting program!!\n";
            break;
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}

