#include "User.h"

#include <cctype>
#include <utility>

namespace {
    std::string RemoveWhitespace(std::string value) {
        std::string result;
        for (char ch : value) {
            if (!std::isspace(static_cast<unsigned char>(ch))) {
                result.push_back(ch);
            }
        }
        return result;
    }
}

User::User(int registrationNumber,
    std::string firstName,
    std::string lastName,
    UserRole role)
    : registrationNumber_(registrationNumber),
    firstName_(std::move(firstName)),
    lastName_(std::move(lastName)),
    role_(role) {
    username_ = BuildUsername(firstName_, lastName_, registrationNumber_, role_);
}

int User::GetRegistrationNumber() const { return registrationNumber_; }
const std::string& User::GetFirstName() const { return firstName_; }
const std::string& User::GetLastName() const { return lastName_; }
const std::string& User::GetUsername() const { return username_; }
UserRole User::GetRole() const { return role_; }
const std::string& User::GetPaymentLastFour() const { return paymentLastFour_; }

void User::SetPaymentLastFour(const std::string& lastFour) {
    paymentLastFour_ = lastFour;
}

std::string User::FullName() const {
    return firstName_ + " " + lastName_;
}

std::string User::RoleText() const {
    switch (role_) {
    case UserRole::Patron: return "Patron";
    case UserRole::Admin: return "Administrator";
    case UserRole::Librarian: return "Librarian";
    case UserRole::Manager: return "Manager";
    }
    return "Unknown";
}

std::string User::RoleCode(UserRole role) {
    switch (role) {
    case UserRole::Patron: return "00";
    case UserRole::Admin: return "01";
    case UserRole::Librarian: return "02";
    case UserRole::Manager: return "03";
    }
    return "00";
}

std::string User::BuildUsername(const std::string& firstName,
    const std::string& lastName,
    int registrationNumber,
    UserRole role) {
    return RemoveWhitespace(lastName) + "." +
        RemoveWhitespace(firstName) +
        std::to_string(registrationNumber) +
        RoleCode(role);
}
