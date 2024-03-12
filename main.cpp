#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <ctime>
#include <cstdlib>

class PasswordManager {
private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> passwords;
    const std::string fileName = "passwords.txt";

public:
    PasswordManager() {
        loadPasswords();
    }

    ~PasswordManager() {
        savePasswords();
    }

    void loadPasswords() {
        std::ifstream file(fileName);
        std::string service, username, password;
        while (file >> service >> username >> password) {
            passwords[service][username] = password;
        }
        file.close();
    }

    void savePasswords() {
        std::ofstream file(fileName);
        for (const auto &service : passwords) {
            for (const auto &user : service.second) {
                file << service.first << " " << user.first << " " << user.second << std::endl;
            }
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
        passwords[service][username] = password;
    }

    std::string getPassword(const std::string& service, const std::string& username) {
        return passwords[service][username];
    }

    void displayAccounts() {
        if (passwords.empty()) {
            std::cout << "No accounts stored.\n";
            return;
        }

        for (const auto& service : passwords) {
            for (const auto& user : service.second) {
                std::cout << "Service: " << service.first << ", Username: " << user.first << std::endl;
            }
        }
    }
};

int main() {
    srand(time(nullptr)); // Seed for random number generation

    PasswordManager pm;

    while (true) {
        std::cout << "Options:\n";
        std::cout << "1. Generate and store a new password\n";
        std::cout << "2. View stored accounts\n";
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
            pm.addOrUpdatePassword(service, username, password); // Store the password

            std::cout << "Generated and stored password for " << service << " and username " << username << ".\n";
        } else if (choice == 2) {
            pm.displayAccounts();
        } else if (choice == 3) {
            break;
        } else {
            std::cout << "Invalid choice. Please enter 1, 2, or 3.\n";
        }
    }

    return 0;
}
