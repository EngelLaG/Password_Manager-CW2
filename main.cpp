#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>

// This will be stored encrypted
struct Account {
    std::string service;
    std::string username;
    std::string password; 
};

class PasswordManager {
private:
    std::vector<Account> accounts;
    const std::string fileName = "passwords.txt";
    const int shift = 3; //shift for Caesar cipher

public:
    PasswordManager() {
        loadPasswords();
    }

    ~PasswordManager() {
        savePasswords();
    }
    //encrypt function
    std::string encrypt(const std::string& input) {
        std::string encrypted = "";
        for (char c : input) {
            if (isalpha(c)) {
                char base = islower(c) ? 'a' : 'A';
                encrypted += char(int(base + (c - base + shift) % 26));
            } else {
                encrypted += c;
            }
        }
        return encrypted;
    }
    //decrypt function
    std::string decrypt(const std::string& input) {
        std::string decrypted = "";
        for (char c : input) {
            if (isalpha(c)) {
                char base = islower(c) ? 'a' : 'A';
                decrypted += char(int(base + (c - base - shift + 26) % 26));
            } else {
                decrypted += c;
            }
        }
        return decrypted;
    }

    void loadPasswords() {
        std::ifstream file(fileName);
        std::string service, username, password;
        while (file >> service >> username >> password) {
            accounts.push_back({decrypt(service), decrypt(username), password}); // Decrypt service and username, keep password encrypted in memory
        }
        file.close();
    }

    void savePasswords() {
        std::ofstream file(fileName);
        for (const auto &account : accounts) {
            // Encrypt service, username, and password before saving
            file << encrypt(account.service) << " "
                 << encrypt(account.username) << " "
                 << account.password << std::endl; // Password is already encrypted
        }
        file.close();
    }

    std::string generatePassword(int length = 12) {
        std::string chars =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "!@#$%^&*()";
        std::string password;
        for (int i = 0; i < length; ++i) {
            password += chars[rand() % chars.length()];
        }
        return password;
    }

    void addOrUpdatePassword(const std::string& service, const std::string& username, const std::string& password) {
        // Check if account exists and update password
        for (auto &account : accounts) {
            if (account.service == service && account.username == username) {
                account.password = encrypt(password);
                return;
            }
        }
        // If not found, add new account with encrypted data
        accounts.push_back({service, username, encrypt(password)});
    }

    void displayAccounts() {
        if (accounts.empty()) {
            std::cout << "No accounts stored.\n";
            return;
        }

        for (const auto& account : accounts) {
            std::cout << "Service: " << account.service
                      << ", Username: " << account.username
                      << ", Password: " << decrypt(account.password) << std::endl; // Decrypt password when displaying
        }
    }
};

int main() {
    srand(time(nullptr)); // Seed for random number generation

    std::string userInput;
    std::cout << "Enter the manager password to access your passwords: ";
    std::cin >> userInput;

    if (userInput != "123") {
        std::cout << "Incorrect password. Exiting...\n";
        return 1; // Exit the program if the password is incorrect
    }

    PasswordManager pm;

    while (true) {
        std::cout << "Options:\n";
        std::cout << "1. Generate and store a new password\n";
        std::cout << "2. View stored accounts and passwords\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            std::string service, username;
            std::cout << "Enter service name: ";
            std::cin >> service;
            std::cout << "Enter username: ";
            std::cin >> username;

            std::string password = pm.generatePassword(12); // Generate a random 12-character password
            pm.addOrUpdatePassword(service, username, password); // Store encrypted password

            std::cout << "Generated and stored password for " << service << " and username " << username << ".\n";
        } else if (choice == 2) {
            pm.displayAccounts(); // Decrypt and display passwords
        } else if (choice == 3) {
            break;
        } else {
            std::cout << "Invalid choice. Please enter 1, 2, or 3.\n";
        }
    }

    return 0;
}