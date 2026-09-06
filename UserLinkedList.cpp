#include "UserLinkedList.h"

#include <algorithm>
#include <cctype>

User* UserLinkedList::AddUser(const std::string& firstName,
    const std::string& lastName,
    UserRole role) {
    auto newUser = std::make_unique<User>(
        nextRegistrationNumber_++, firstName, lastName, role);
    User* rawUser = newUser.get();
    auto newNode = std::make_unique<Node>(std::move(newUser));

    if (!head_) {
        tail_ = newNode.get();
        head_ = std::move(newNode);
    }
    else {
        tail_->next = std::move(newNode);
        tail_ = tail_->next.get();
    }

    ++size_;
    return rawUser;
}

User* UserLinkedList::FindByUsername(const std::string& username) {
    Node* current = head_.get();
    while (current != nullptr) {
        if (current->user->GetUsername() == username) {
            return current->user.get();
        }
        current = current->next.get();
    }
    return nullptr;
}

const User* UserLinkedList::FindByUsername(const std::string& username) const {
    const Node* current = head_.get();
    while (current != nullptr) {
        if (current->user->GetUsername() == username) {
            return current->user.get();
        }
        current = current->next.get();
    }
    return nullptr;
}

std::vector<User*> UserLinkedList::FindByNameContains(const std::string& text) {
    const std::string needle = Lower(text);
    std::vector<User*> matches;

    Node* current = head_.get();
    while (current != nullptr) {
        const std::string searchable = Lower(
            current->user->FullName() + " " + current->user->GetUsername());
        if (searchable.find(needle) != std::string::npos) {
            matches.push_back(current->user.get());
        }
        current = current->next.get();
    }
    return matches;
}

std::vector<const User*> UserLinkedList::GetAllUsers() const {
    std::vector<const User*> users;
    users.reserve(size_);

    const Node* current = head_.get();
    while (current != nullptr) {
        users.push_back(current->user.get());
        current = current->next.get();
    }
    return users;
}

bool UserLinkedList::RemoveByUsername(const std::string& username) {
    std::unique_ptr<Node>* link = &head_;

    while (*link != nullptr) {
        if ((*link)->user->GetUsername() == username) {
            Node* removed = link->get();
            if (removed == tail_) {
                tail_ = nullptr;
                Node* scan = head_.get();
                while (scan != nullptr && scan != removed) {
                    tail_ = scan;
                    scan = scan->next.get();
                }
            }
            *link = std::move((*link)->next);
            --size_;
            if (!head_) tail_ = nullptr;
            return true;
        }
        link = &((*link)->next);
    }
    return false;
}

bool UserLinkedList::ValidateUsernameForStoredRole(const User& user) const {
    const std::string& username = user.GetUsername();
    if (username.size() < 2) return false;

    const std::string actualCode = username.substr(username.size() - 2);
    const std::string expectedCode = User::RoleCode(user.GetRole());
    const std::string expectedUsername = User::BuildUsername(
        user.GetFirstName(), user.GetLastName(),
        user.GetRegistrationNumber(), user.GetRole());

    return actualCode == expectedCode && username == expectedUsername;
}

std::size_t UserLinkedList::Size() const { return size_; }
int UserLinkedList::NextRegistrationNumber() const { return nextRegistrationNumber_; }

std::string UserLinkedList::Lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return value;
}
