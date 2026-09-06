#pragma once

#include "UserLinkedList.h"
#include "BookInventory.h"

#include <string>

class LibrarySystem {
public:
    LibrarySystem();
    void Run();

private:
    BookInventory inventory_;
    UserLinkedList users_;

    void SeedUsers();
    User* Authenticate();

    void ShowcaseMode();

    void PatronMenu(User& user);
    void EmployeeMenu(User& user);
    void AdminMenu(User& user);

    void ShowSelfAccount(const User& user) const;
    void EditPaymentInfo(User& user);

    void InventoryLookupMenu() const;
    void ShowInventorySummary() const;
    void DisplayEntireCatalog() const;
    void DisplayBook(const Book& book) const;
    void DisplayBooks(const std::vector<Book*>& books, std::size_t maxRows = 25) const;

    void AddBookInteractive();
    void RemoveBookInteractive();

    void AddUserInteractive();
    void FindUserInteractive();
    void ListUsers() const;
    void RemoveUserInteractive(const User& signedInAdmin);

    static int ReadInt(const std::string& prompt, int minValue, int maxValue);
    static double ReadDouble(const std::string& prompt, double minValue);
    static std::string ReadLine(const std::string& prompt);
    static UserRole ReadRole();

};

