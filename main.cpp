#include <iostream>
#include <fstream>
#include <unordered_map>
#include <ctime>
#include <cstdlib>

class PasswordManager {
private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> passwords;
    const std::string fileName = "passwords.txt";
    const int shift = 3; // Fixed shift for Caesar cipher

public:
    PasswordManager() {
        loadPasswords();
    }

    ~PasswordManager() {
        savePasswords();
    }

    std::string encrypt(const std::string& input) {
        std::string encrypted = "";
        for (char c : input) {
            if (isalpha(c)) {
                char base = islower(c) ? 'a' : 'A';
                encrypted += char(int(base + (c - base + shift) % 26));
            } else {
                encrypted += c; // Non-alphabetic characters are not encrypted
            }
        }
        return encrypted;
    }

    std::string decrypt(const std::string& input) {
        std::string decrypted = "";
        for (char c : input) {
            if (isalpha(c)) {
                char base = islower(c) ? 'a' : 'A';
                decrypted += char(int(base + (c - base - shift + 26) % 26));
            } else {
                decrypted += c; // Non-alphabetic characters are not encrypted
            }
        }
        return decrypted;
    }

    void loadPasswords() {
        std::ifstream file(fileName);
        std::string service, username, encryptedPassword;
        while (file >> service >> username >> encryptedPassword) {
            passwords[service][username] = encryptedPassword; // Keep passwords encrypted in memory
        }
        file.close();
    }

    void savePasswords() {
        std::ofstream file(fileName);
        for (const auto &service : passwords) {
            for (const auto &user : service.second) {
                file << service.first << " " << user.first << " " << user.second << std::endl; // Store encrypted
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
        passwords[service][username] = encrypt(password); // Encrypt before storing
    }

    void displayAccounts() {
        if (passwords.empty()) {
            std::cout << "No accounts stored.\n";
            return;
        }

        for (const auto& service : passwords) {
            for (const auto& user : service.second) {
                std::cout << "Service: " << service.first << ", Username: " << user.first << ", Password: " << decrypt(user.second) << std::endl; // Decrypt when displaying
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
            pm.addOrUpdatePassword(service, username, password); // Encrypt and store the password

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
