#pragma once

#include <string>

enum class UserRole {
    Patron,
    Admin,
    Librarian,
    Manager
};

class User {
public:
    User(int registrationNumber,
        std::string firstName,
        std::string lastName,
        UserRole role);

    int GetRegistrationNumber() const;
    const std::string& GetFirstName() const;
    const std::string& GetLastName() const;
    const std::string& GetUsername() const;
    UserRole GetRole() const;
    const std::string& GetPaymentLastFour() const;

    void SetPaymentLastFour(const std::string& lastFour);

    std::string FullName() const;
    std::string RoleText() const;

    static std::string RoleCode(UserRole role);
    static std::string BuildUsername(const std::string& firstName,
        const std::string& lastName,
        int registrationNumber,
        UserRole role);

private:
    int registrationNumber_;
    std::string firstName_;
    std::string lastName_;
    std::string username_;
    UserRole role_;
    std::string paymentLastFour_ = "Not set";
};
