#pragma once

#include "User.h"

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Custom singly linked list required by the project for user management.
class UserLinkedList {
public:
    UserLinkedList() = default;
    ~UserLinkedList() = default;

    UserLinkedList(const UserLinkedList&) = delete;
    UserLinkedList& operator=(const UserLinkedList&) = delete;

    User* AddUser(const std::string& firstName,
        const std::string& lastName,
        UserRole role);

    User* FindByUsername(const std::string& username);
    const User* FindByUsername(const std::string& username) const;

    std::vector<User*> FindByNameContains(const std::string& text);
    std::vector<const User*> GetAllUsers() const;

    bool RemoveByUsername(const std::string& username);
    bool ValidateUsernameForStoredRole(const User& user) const;

    std::size_t Size() const;
    int NextRegistrationNumber() const;

private:
    struct Node {
        explicit Node(std::unique_ptr<User> newUser)
            : user(std::move(newUser)) {
        }

        std::unique_ptr<User> user;
        std::unique_ptr<Node> next;
    };

    std::unique_ptr<Node> head_;
    Node* tail_ = nullptr;
    std::size_t size_ = 0;
    int nextRegistrationNumber_ = 1;

    static std::string Lower(std::string value);
};
