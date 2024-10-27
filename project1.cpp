#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Employee {
    string name;
    int nKeysPossessed;
    string keys[5];
};

bool reader(string input_filename, Employee employees[], int& nEmployees) {
    ifstream ifile(input_filename);
    
    if (! ifile.is_open()) {
        return false;
    }

    ifile >> nEmployees;
    for (int i = 0; i < nEmployees; ++i) {
        ifile.ignore();                                                                             // ignore next char (\n) before getline to remove it from file input buffer so getline actually takes the info from the next line
        getline(ifile, employees[i].name);
        ifile >> employees[i].nKeysPossessed;
        for (int j = 0; j < employees[i].nKeysPossessed; ++j) {
            ifile >> employees[i].keys[j];
        }
    }
    ifile.close();

    return true;
}

void writer(string output_filename, Employee employees[], int nEmployees) {
    ofstream ofile(output_filename);

    ofile << nEmployees << endl;
    for (int i = 0; i < nEmployees; ++i) {
        ofile << employees[i].name << endl;
        ofile << employees[i].nKeysPossessed << " ";
        for (int j = 0; j < employees[i].nKeysPossessed; ++j) {
            if (j == employees[i].nKeysPossessed - 1) {                                             // last key
                ofile << employees[i].keys[j] << endl;
                continue;
            }
            ofile << employees[i].keys[j] << " ";
        }
    }

    ofile.close();
}

bool addKeyForEmployee(Employee employees[], int nEmployees, string emp_name, string newKey) {
    for (int i = 0; i < nEmployees; ++i) {
        if (employees[i].name == emp_name) {
            if (employees[i].nKeysPossessed == 5) {
                cout << "This employee already has 5 keys!" << endl << endl;
                return false;
            }
            else if (employees[i].nKeysPossessed < 5) {
                for (int j = 0; j < employees[i].nKeysPossessed; ++j) {
                    if (employees[i].keys[j] == newKey) {
                        cout << "This employee already has this key!" << endl << endl;
                        return false;
                    }
                }
                employees[i].keys[employees[i].nKeysPossessed] = newKey;
                ++employees[i].nKeysPossessed;
                return true;
            }
        }
    }
    cout << "Cannot find the specified employee!" << endl << endl;
    return false;
}

bool returnAKey(Employee employees[], int nEmployees, string emp_name, string returnKey) {
    for (int i = 0; i < nEmployees; ++i) {
        if (employees[i].name == emp_name) {
            for (int j = 0; j < employees[i].nKeysPossessed; ++j) {
                if (employees[i].keys[j] == returnKey) {
                    for (int k = j; k < employees[i].nKeysPossessed - 1; ++k) {                     // shift all keys down starting from returnKey through the second to last key
                        employees[i].keys[k] = employees[i].keys[k+1];
                    }
                    --employees[i].nKeysPossessed;                                                  // last key (empty) never accessed
                    return true;
                }
            }
            cout << "This employee does not have the specified key!" << endl << endl;
            return false;
        }
    }
    cout << "Cannot find the specified employee!" << endl << endl;
    return false;
}

int main() {
    system("clear");

    // get file name
    cout << "Please enter key file name to start: ";
    string filename;
    cin >> filename;

    // load file into array of employees
    Employee employees[10];
    int nEmployees = 0;
    if (! reader(filename, employees, nEmployees)) {
        cout << "File not found, exiting the program..." << endl;
        return 1;
    }

    // execute menu options
    bool cont = true;
    while (cont) {
        cout << endl << "Please select from the following options: \n  1. show all employees and their keys\n  2. show the keys an employee possesses\n  3. show which employees possess a specific key\n  4. add a key to an employee\n  5. return a key by an employee\n  6. save the current key status\n  7. exit the program\n";
        int option;
        cin >> option;
        
        if (option == 1) {
            for (int i = 0; i < nEmployees; ++i) {
                cout << "Name: " << employees[i].name << endl;
                cout << "Keys possessed: ";
                for (int j = 0; j < employees[i].nKeysPossessed; ++j) {
                    cout << employees[i].keys[j] << " ";
                }
                cout << endl;
            }
            cout << endl;
            continue;
        }
        
        else if (option == 2) {
            cout << "Please enter employee's name: ";
            string emp_name;
            cin.ignore();
            getline(cin, emp_name);

            vector<string> keysForEmp;
            for (int i = 0; i < nEmployees; ++i) {
                if (employees[i].name == emp_name) {
                    for (int j = 0; j < employees[i].nKeysPossessed; ++j) {
                        keysForEmp.push_back(employees[i].keys[j]);
                    }
                }
            }

            if (keysForEmp.size() == 0) {
                cout << "Cannot find the specified employee!" << endl << endl;
                continue;
            }
            else {
                cout << emp_name << " possesses the following keys: ";
                for (int i = 0; i < keysForEmp.size(); ++i) {
                    cout << keysForEmp.at(i) << " ";
                }
                cout << endl << endl;
                continue;
            }
        }
        
        else if (option == 3) {
            cout << "Please enter a key: ";
            string key;
            cin >> key;

            vector<string> empWithKeys;
            for (int i = 0; i < nEmployees; ++i) {
                for (int j = 0; j < employees[i].nKeysPossessed; ++j) {
                    if (employees[i].keys[j] == key) {
                        empWithKeys.push_back(employees[i].name);
                    }
                }
            }

            if (empWithKeys.size() == 0) {
                cout << "No one possesses this key." << endl << endl;
                continue;
            }
            else {
                for (int i = 0; i < empWithKeys.size(); ++i) {
                    cout << empWithKeys.at(i) << ", ";
                }
                cout << "possess this key." << endl << endl;
                continue;
            }
        }
        
        else if (option == 4) {
            cout << "Please enter employee's name: ";
            string emp_name;
            cin.ignore();
            getline(cin, emp_name);
            cout << "Please enter a new key: ";
            string new_key;
            cin >> new_key;

            if (addKeyForEmployee(employees, nEmployees, emp_name, new_key)) {
                cout << "Key added successfully." << endl << endl;
                continue;
            }
        }
        
        else if (option == 5) {
            cout << "Please enter employee's name: ";
            string emp_name;
            cin.ignore();
            getline(cin, emp_name);
            cout << "Please enter the returned key: ";
            string ret_key;
            cin >> ret_key;

            if (returnAKey(employees, nEmployees, emp_name, ret_key)) {
                cout << "Key returned successfully." << endl << endl;
            }
            continue;
        }
        
        else if (option == 6) {
            cout << "Please enter output file name: ";
            string save_file;
            cin.ignore();
            getline(cin, save_file);

            writer(save_file, employees, nEmployees);

            cout << "Key information saved to " << save_file << endl << endl;

            continue;
        }
        
        else if (option == 7) {
            writer("keys_updated.txt", employees, nEmployees);
            cout << "Thank you for using the system! Goodbye!" << endl;
            cont = false;
            continue;
        }
       
        else {
            cout << "Not a valid option. Please try again." << endl << endl;
            continue;
        }
    }

    return 0;
}