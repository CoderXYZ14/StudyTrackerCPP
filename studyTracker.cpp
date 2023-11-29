#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

const int MAX_USERS = 100;
const int MAX_SUBJECTS = 10;
const int MAX_SUBJECT_LENGTH = 50;
const int MAX_HISTORY_LENGTH = 100;


class User{
  public :   
    string username;
    string password;
    string email;
    string enrolledSubjects[MAX_SUBJECTS];

    User(const string& uname, const string& pword) : username(uname), password(pword)
    {

    }
    User()
    {
        
    }

    void addEnrolledSubject(const string &subject){
        for (int i = 0; i < MAX_SUBJECTS; i++){
            if(enrolledSubjects[i].empty()){
                enrolledSubjects[i] = subject;
                break;
            }
        }
        
    }
};

class UserManager{
  private:
    User users[MAX_USERS];
    User* currentUser;

    const string userFile = "users.txt";

    void loadUsers() {
        ifstream file(userFile);
        if(file.is_open()){
            string uname, pword;
            int userCount = 0;
            while(file >> uname >> pword){
                users[userCount++] = User(uname, pword);
            }
            file.close();
        }
    }
     
    void saveUsers() {
        ofstream file(userFile);
        if (file.is_open()){
            for(int i = 0; i<MAX_USERS && !users[i].username.empty(); ++i){
                file << "USER NAME : " <<users[i].username << "||" <<"PASSWORD : " << users[i].password<<"\n";
            }
            file.close();
        }
    }

  public :
    UserManager() : currentUser(nullptr) {
        loadUsers();
    }

    void registerUser(string &uname, string &pword){
        for(int i = 0; i<MAX_USERS; ++i){
            if(users[i].username.empty())1{
                users[i] = User(uname, pword);
                saveUsers();
                cout << "Registeration successfull...\n";
                return;
            }
        }
        cout << "Cannot register, maximum number of users reached.\n";
    }

    bool loginUser(string uname, string pword){
        for (int i = 0; i < MAX_USERS && !users[i].username.empty(); ++i) {
            if (users[i].username == uname && users[i].password == pword) {
                std::cout << "Login successful. Welcome, " << uname << "!\n";
                currentUser = &users[i];
                return true;
            }
        }
        std::cout << "Invalid username or password. Login failed.\n";
        return false;
    }

    void logoutUser(){
        currentUser = nullptr;
        std::cout << "Logout successful.\n";
    }

    bool isUserLoggedIn() const {
        return currentUser != nullptr;
    }

    User* getCurrentUser() {
        return currentUser;
    }
};

class Timer {
  private : 
    time_t startTime;
    time_t stopTime;

  public :
    void startTimer(){
        startTime = time(nullptr);
    }
    void stopTimer(){
        stopTime = time(nullptr);
    }
    int getElapsedTime() const {
        return static_cast<int>(stopTime - startTime);
    }
};

void readTopics(User *user, Timer &timer){
    if(user == nullptr){
        cout << "Please login first.\n";
        return;
    }

    string subject, topic;
    cout << "---------------------------Reading-------------------------------\n";
    cout << "Enter Subject : ";
    cin >> subject;

    user->addEnrolledSubject(subject);

    cout<< "Enter Topic : ";
    cin >> topic;

    cout<<"Timer Started. Press any key to stop\n";
    cout << "-----------------------------------------------------------------\n";
    timer.startTimer();

    cin.ignore();
    cin.get();
    timer.stopTimer();

    int elapsedTime = timer.getElapsedTime();
    cout << "You spent " << elapsedTime << " seconds on " << subject << ": " << topic << ".\n";

    ofstream historyFile("history.txt", std::ios::app);
    if (historyFile.is_open()) {
        historyFile << "User: " << user->username << " || Subject: " << subject << " || Topic: " << topic
                    << " || Time Spent: " << elapsedTime << " seconds\n";
        historyFile.close();
    }
}

int main() {
    UserManager userManager;
    Timer timer;

    while (true) {
        cout << "\n\n\n-----------------------------------------------------------------\n---------------------------MAIN MENU----------------------------\n| 1. Register | 2. Login | 3. Logout | 4. Read Topics | 5. Exit |\n-----------------------------------------------------------------\n-----------------------------------------------------------------\n\n\n";
        int choice;
        cin >> choice;

        if (choice == 1) {
            string uname, pword;
            cout << "----------------Registration----------------\n";
            cout << "Enter username: ";
            cin >> uname;
            cout << "Enter password: ";
            cin >> pword;
            userManager.registerUser(uname, pword);
            cout << "---------------------------------------------\n";
        } else if (choice == 2) {
            string uname, pword;
            cout << "-------------------LOGIN---------------------\n";
            cout << "Enter username: ";
            cin >> uname;
            cout << "Enter password: ";
            cin >> pword;
            userManager.loginUser(uname, pword);
            cout << "---------------------------------------------\n";
        } else if (choice == 3) {
            userManager.logoutUser();
        } else if (choice == 4) {
            readTopics(userManager.getCurrentUser(), timer);
        } else if (choice == 5) {
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}