#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>


using namespace std;


struct Expense {
    string category;
    double price;
    string description;
    string date;

    Expense(){
        auto now = chrono::system_clock::now();
        time_t nowTime = chrono::system_clock::to_time_t(now);
        tm now_tm = *localtime(&nowTime);
        date = to_string(now_tm.tm_mday) + "." + to_string(now_tm.tm_mon + 1) + "." + to_string(
                          now_tm.tm_year + 1900);
    }

    void display() {
        cout << "Category: " << category << ", ";
        cout << "Price: " << price << ", ";
        cout << "Description: " << description << ", ";
        cout << "Date: " << date << endl;
    }

    void input() {
        cout << "Category: ";
        cin >> category;
        cout << "Price: ";
        cin >> price;
        cout << "Description: ";
        cin.ignore();
        getline(cin, description);
    }
};


void writeToFile(const string &fileName, const string &content) {
    ofstream file(fileName, ios::app);

    if (file.is_open()) {
        file << content << endl;
        file.close();
        cout << "Contents written to file " << fileName << endl;
    }
    else {
        cout << "Unable to open file " << fileName << endl;
    }
}


vector<Expense> readFromFile(const string &fileName) {
    vector<Expense> expenses;
    ifstream file(fileName);
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            istringstream iss(line);
            Expense expense;
            iss >> expense.category >> expense.price;
            iss.ignore();
            getline(iss, expense.description, '|');
            getline(iss, expense.date);
            expenses.push_back(expense);
        }
    }
    file.close();
    return expenses;
}


int gradeDate(const string &date) {
    int day, month, year;
    sscanf(date.c_str(), "%d.%d.%d", &day, &month, &year);
    return year * 10000 + month * 100 + day;
}


void sortExpensesByDate(vector<Expense> &expenses) {
    sort(expenses.begin(), expenses.end(), [](const Expense &e1, const Expense &e2) {
       return gradeDate(e1.date) < gradeDate(e2.date);
    });
}


double sumOfAllExpenses(const vector<Expense> &expenses) {
    double sum = 0;
    for (int i = 0; i < expenses.size(); i++) {
        sum += expenses[i].price;
    }
    return sum;
}


vector<Expense> filterByCategory(const vector<Expense> &expenses, const string &category) {
    vector<Expense> filtered;
    string expense_category;
    string lower_category = category;
    transform(lower_category.begin(), lower_category.end(), lower_category.begin(), ::tolower);
    for (const auto &expense : expenses) {
        string expense_category = expense.category;
        transform(expense_category.begin(), expense_category.end(), expense_category.begin(), ::tolower);
        if (lower_category == expense_category) {
            filtered.push_back(expense);
        }
    }
    return filtered;
}


int main() {
    string fileName = "database.txt";

    vector<Expense> expenses = readFromFile(fileName);
    short task;
    cout << "Enter the task number:\n"
            "type '0' to exit\n '1' to input an expense to the file\n '2' to display all expenses sorted by date\n '3' to display the sum of all your expenses\n '4' to display all expenses in a specific category\n ";

    while (true) {
        cin >> task;
        switch (task) {
            case 0: {
                cout << "Goodbye, wishing you new expenses. :)" << endl;
                return 0;
            }
            case 1: {
                Expense expense;
                expense.input();
                string input = expense.category + ' ' + to_string(expense.price) + ' ' + expense.description + '|' + expense.date;
                writeToFile(fileName, input);
                expenses = readFromFile(fileName);
                break;
            }
            case 2: {
                cout << "All your expenses sorted by date:" << endl;
                sortExpensesByDate(expenses);
                for (auto expense : expenses) {
                    expense.display();
                }
                break;
            }
            case 3: {
                cout << "Sum of all your expenses: " << sumOfAllExpenses(expenses) << endl;
                break;
            }
            case 4: {
                string category;
                cout << "Enter the category: ";
                cin >> category;
                vector<Expense> filtered = filterByCategory(expenses, category);
                if (filtered.empty()) {
                    cout << "No category found" << endl;
                } else {
                    for (auto &expense : filtered) {
                        expense.display();
                    }
                }
                break;
            }
            default: {
                cout << "Wrong task number! Please try again another task number." << endl;
            }
        }
        cout << "Enter the task number: ";
    }

}