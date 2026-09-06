#include "LibrarySystem.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>

LibrarySystem::LibrarySystem() {
    inventory_.GenerateDemoBooks(5000);
    SeedUsers();
}

/*void LibrarySystem::Run() {
    std::cout << "\nSMART LIBRARY MANAGEMENT SYSTEM - Aron Littlejohn - CPSC 315 \n";
    std::cout << "Catalog: " << inventory_.TotalBooks() << " books\n";

    while (true) {
        std::cout << "\n1. Log in\n9.Demo/ShowcaseAllFeatures\n0. Exit\n";
        const int choice = ReadInt("Selection: ", 0, 1);
        if (choice == 0) {
            std::cout << "Goodbye.\n";
            return;
        }

        User* user = Authenticate();
        if (user == nullptr) {
            std::cout << "Authentication failed. Username not found or role code invalid.\n";
            continue;
        }

        std::cout << "\nWelcome, " << user->FullName()
            << " (" << user->RoleText() << ").\n";

        switch (user->GetRole()) {
        case UserRole::Patron:
            PatronMenu(*user);
            break;
        case UserRole::Librarian:
        case UserRole::Manager:
            EmployeeMenu(*user);
            break;
        case UserRole::Admin:
            AdminMenu(*user);
            break;
        }
    }
}*/
void LibrarySystem::Run() {
    std::cout << "\nSMART LIBRARY MANAGEMENT SYSTEM - Aron Littlejohn - CPSC 315 \n";
    std::cout << "Catalog: " << inventory_.TotalBooks() << " books\n";

    while (true) {
        std::cout << "\n============================================\n";
        std::cout << "       SMART LIBRARY MAIN MENU\n";
        std::cout << "============================================\n";
        std::cout << "1. Log in\n";
        std::cout << "6. Display Entire Book Catalog\n";
        std::cout << "9. Demo / Showcase All Features\n";
        std::cout << "0. Exit\n";

        const int choice = ReadInt("Selection: ", 0, 9);

        if (choice == 0) {
            std::cout << "Goodbye.\n";
            return;
        }

        // Display every book without requiring login.
        if (choice == 6) {
            DisplayEntireCatalog();
            continue;
        }

        // Display all system features without requiring login.
        if (choice == 9) {
            ShowcaseMode();
            continue;
        }

        // Only 0, 1, 6, and 9 are valid main-menu choices.
        if (choice != 1) {
            std::cout << "Invalid selection. Please choose 0, 1, 6, or 9.\n";
            continue;
        }

        User* user = Authenticate();

        if (user == nullptr) {
            std::cout << "Authentication failed. Username not found or role code invalid.\n";
            continue;
        }

        std::cout << "\nWelcome, " << user->FullName()
            << " (" << user->RoleText() << ").\n";

        switch (user->GetRole()) {

        case UserRole::Patron:
            PatronMenu(*user);
            break;

        case UserRole::Librarian:
        case UserRole::Manager:
            EmployeeMenu(*user);
            break;

        case UserRole::Admin:
            AdminMenu(*user);
            break;
        }
    }
}

void LibrarySystem::SeedUsers() {
    users_.AddUser("Admin", "System", UserRole::Admin);
    users_.AddUser("Marcus", "Houston", UserRole::Patron);
    users_.AddUser("Leah", "Morgan", UserRole::Librarian);
    users_.AddUser("Daniel", "Brooks", UserRole::Manager);
}

User* LibrarySystem::Authenticate() {
    std::cout << "\nWHO ARE YOU?\n"
        << "1. Patron\n"
        << "2. Administrator\n"
        << "3. Employee (librarian or manager)\n"
        << "0. Cancel\n";
    const int accountType = ReadInt("Selection: ", 0, 3);
    if (accountType == 0) return nullptr;

    const std::string username = ReadLine("Username: ");
    User* user = users_.FindByUsername(username);
    if (user == nullptr || !users_.ValidateUsernameForStoredRole(*user)) {
        return nullptr;
    }

    const bool categoryMatches =
        (accountType == 1 && user->GetRole() == UserRole::Patron) ||
        (accountType == 2 && user->GetRole() == UserRole::Admin) ||
        (accountType == 3 &&
            (user->GetRole() == UserRole::Librarian ||
                user->GetRole() == UserRole::Manager));

    return categoryMatches ? user : nullptr;
}

void LibrarySystem::PatronMenu(User& user) {
    while (true) {
        std::cout << "\nPATRON MENU\n"
            << "1. View my account\n"
            << "2. Edit my payment information\n"
            << "3. Search library inventory\n"
            << "4. Browse books sorted by title\n"
            << "0. Log out\n";

        const int choice = ReadInt("Selection: ", 0, 4);
        switch (choice) {
        case 1: ShowSelfAccount(user); break;
        case 2: EditPaymentInfo(user); break;
        case 3: InventoryLookupMenu(); break;
        case 4: DisplayBooks(inventory_.GetBooksSortedByTitle()); break;
        case 0: return;
        }
    }
}

void LibrarySystem::EmployeeMenu(User& user) {
    while (true) {
        std::cout << "\nEMPLOYEE MENU - " << user.RoleText() << "\n"
            << "1. Patron functions\n"
            << "2. Search inventory / view book location\n"
            << "3. Inventory summary\n"
            << "4. Add book\n"
            << "5. Remove book\n"
            << "6. Display books sorted by title\n"
            << "7. Display books sorted by author\n"
            << "8. Display books sorted by publication year\n"
            << "0. Log out\n";

        const int choice = ReadInt("Selection: ", 0, 8);
        switch (choice) {
        case 1: PatronMenu(user); break;
        case 2: InventoryLookupMenu(); break;
        case 3: ShowInventorySummary(); break;
        case 4: AddBookInteractive(); break;
        case 5: RemoveBookInteractive(); break;
        case 6: DisplayBooks(inventory_.GetBooksSortedByTitle()); break;
        case 7: DisplayBooks(inventory_.GetBooksSortedByAuthor()); break;
        case 8: DisplayBooks(inventory_.GetBooksSortedByYear()); break;
        case 0: return;
        }
    }
}

void LibrarySystem::AdminMenu(User& user) {
    while (true) {
        std::cout << "\nADMIN MENU\n"
            << "1. View my account\n"
            << "2. Search inventory\n"
            << "3. Inventory summary\n"
            << "4. Add user account\n"
            << "5. Search user account\n"
            << "6. List all users\n"
            << "7. Remove user account\n"
            << "0. Log out\n";

        const int choice = ReadInt("Selection: ", 0, 7);
        switch (choice) {
        case 1: ShowSelfAccount(user); break;
        case 2: InventoryLookupMenu(); break;
        case 3: ShowInventorySummary(); break;
        case 4: AddUserInteractive(); break;
        case 5: FindUserInteractive(); break;
        case 6: ListUsers(); break;
        case 7: RemoveUserInteractive(user); break;
        case 0: return;
        }
    }
}

void LibrarySystem::ShowSelfAccount(const User& user) const {
    std::cout << "\nACCOUNT DETAILS\n"
        << "Name: " << user.FullName() << "\n"
        << "Registration #: " << user.GetRegistrationNumber() << "\n"
        << "Username: " << user.GetUsername() << "\n"
        << "Role: " << user.RoleText() << "\n"
        << "Payment method: ";

    if (user.GetPaymentLastFour() == "Not set") {
        std::cout << "Not set\n";
    }
    else {
        std::cout << "Card ending in " << user.GetPaymentLastFour() << "\n";
    }

   // std::cout << "Checkout history, due dates, recommendations, and overdue fees "
     //   << "are Module 5 features.\n";
}

void LibrarySystem::EditPaymentInfo(User& user) {
    const std::string lastFour = ReadLine("Enter the last 4 digits of the payment card: ");
    if (lastFour.size() != 4 ||
        lastFour.find_first_not_of("0123456789") != std::string::npos) {
        std::cout << "Payment information was not changed. Enter exactly 4 digits.\n";
        return;
    }

    // Only the last four digits are stored in this build.  Full
    // payment processing/storage would require a secure external payment system.
    user.SetPaymentLastFour(lastFour);
    std::cout << "Payment display information updated.\n";
}

void LibrarySystem::InventoryLookupMenu() const {
    std::cout << "\nINVENTORY SEARCH\n"
        << "1. Exact ISBN (hash lookup)\n"
        << "2. Exact title (hash lookup)\n"
        << "3. Title contains text (linear search)\n";
    const int choice = ReadInt("Selection: ", 1, 3);

    if (choice == 1) {
        const std::string isbn = ReadLine("ISBN: ");
        const Book* book = inventory_.FindByIsbn(isbn);
        if (book) DisplayBook(*book);
        else std::cout << "No book found.\n";
    }
    else if (choice == 2) {
        const std::string title = ReadLine("Exact title: ");
        const auto matches = inventory_.FindByExactTitle(title);
        if (matches.empty()) std::cout << "No book found.\n";
        else DisplayBooks(matches, 25);
    }
    else {
        const std::string text = ReadLine("Title text: ");
        const auto matches = inventory_.SearchTitleContains(text);
        if (matches.empty()) std::cout << "No book found.\n";
        else DisplayBooks(matches, 25);
    }
}

void LibrarySystem::ShowInventorySummary() const {
    std::cout << "\nINVENTORY SUMMARY\n"
        << "Catalog records: " << inventory_.TotalBooks() << "\n"
        << "Available: " << inventory_.AvailableBooks() << "\n"
        << "Unavailable: " << inventory_.UnavailableBooks() << "\n"
        << "ISBN hash-table load factor: " << std::fixed << std::setprecision(3)
        << inventory_.IsbnLoadFactor() << "\n"
        << "Title hash-table load factor: " << inventory_.TitleLoadFactor() << "\n";
}

void LibrarySystem::DisplayBook(const Book& book) const {
    std::cout << "\nCatalog ID: " << book.GetCatalogId() << "\n"
        << "ISBN: " << book.GetIsbn() << "\n"
        << "Title: " << book.GetTitle() << "\n"
        << "Author: " << book.GetAuthor() << "\n"
        << "Publisher: " << book.GetPublisher() << "\n"
        << "Publication year: " << book.GetPublicationYear() << "\n"
        << "Illustrator: " << book.GetIllustrator() << "\n"
        << "Retail price: $" << std::fixed << std::setprecision(2)
        << book.GetRetailPrice() << "\n"
        << "Status: " << book.StatusText() << "\n"
        << "Location: " << book.GetLocation().ToString() << "\n";
}

void LibrarySystem::DisplayBooks(const std::vector<Book*>& books, std::size_t maxRows) const {
    const std::size_t displayCount = std::min(books.size(), maxRows);
    std::cout << "\nShowing " << displayCount << " of " << books.size() << " matching books:\n";
    std::cout << std::left
        << std::setw(15) << "ISBN"
        << std::setw(38) << "Title"
        << std::setw(24) << "Author"
        << std::setw(8) << "Year"
        << std::setw(12) << "Status"
        << "Location\n";
    std::cout << std::string(120, '-') << "\n";

    for (std::size_t i = 0; i < displayCount; ++i) {
        const Book& book = *books[i];
        std::string title = book.GetTitle();
        std::string author = book.GetAuthor();
        if (title.size() > 35) title = title.substr(0, 32) + "...";
        if (author.size() > 21) author = author.substr(0, 18) + "...";

        std::cout << std::left
            << std::setw(15) << book.GetIsbn()
            << std::setw(38) << title
            << std::setw(24) << author
            << std::setw(8) << book.GetPublicationYear()
            << std::setw(12) << book.StatusText()
            << book.GetLocation().ToString() << "\n";
    }

    if (books.size() > displayCount) {
        std::cout << "Output limited to " << displayCount
            << " rows to keep the console readable.\n";
    }
}
void LibrarySystem::DisplayEntireCatalog() const
{
    std::cout << "\n=====================================================\n";
    std::cout << "              COMPLETE BOOK CATALOG\n";
    std::cout << "=====================================================\n";

    std::cout << "Total books in catalog: "
        << inventory_.TotalBooks() << "\n";

    // Get every book sorted alphabetically by title.
    const std::vector<Book*> books = inventory_.GetBooksSortedByTitle();

    // DisplayBooks normally only shows 25 rows.
    // Passing books.size() allows every book to be displayed.
    DisplayBooks(books, books.size());

    std::cout << "\n=====================================================\n";
    std::cout << "               END OF CATALOG\n";
    std::cout << "=====================================================\n";

    std::cout << "\nPress ENTER to return to the main menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
void LibrarySystem::AddBookInteractive() {
    try {
        const std::string isbn = ReadLine("ISBN: ");
        const std::string title = ReadLine("Title: ");
        const std::string author = ReadLine("Author: ");
        const std::string publisher = ReadLine("Publisher: ");
        const int year = ReadInt("Publication year: ", 0, 2100);
        const std::string illustrator = ReadLine("Illustrator (or N/A): ");
        const double price = ReadDouble("Retail price: $", 0.0);

        Book* book = inventory_.AddBook(
            isbn, title, author, publisher, year, illustrator, price);
        std::cout << "Book added. Catalog ID " << book->GetCatalogId()
            << ", location: " << book->GetLocation().ToString() << "\n";
    }
    catch (const std::exception& ex) {
        std::cout << "Book was not added: " << ex.what() << "\n";
    }
}

void LibrarySystem::RemoveBookInteractive() {
    const std::string isbn = ReadLine("ISBN to remove: ");
    if (inventory_.RemoveBookByIsbn(isbn)) {
        std::cout << "Book removed from the catalog. Its shelf slot is now available.\n";
    }
    else {
        std::cout << "No book with that ISBN was found.\n";
    }
}

void LibrarySystem::AddUserInteractive() {
    const std::string firstName = ReadLine("First name: ");
    const std::string lastName = ReadLine("Last name: ");
    const UserRole role = ReadRole();

    User* user = users_.AddUser(firstName, lastName, role);
    std::cout << "User added. Registration #: " << user->GetRegistrationNumber()
        << "\nGenerated username: " << user->GetUsername() << "\n";
}

void LibrarySystem::FindUserInteractive() {
    const std::string text = ReadLine("Name or username text: ");
    const auto matches = users_.FindByNameContains(text);
    if (matches.empty()) {
        std::cout << "No matching users.\n";
        return;
    }

    for (const User* user : matches) {
        std::cout << user->GetRegistrationNumber() << " | "
            << user->FullName() << " | "
            << user->RoleText() << " | "
            << user->GetUsername() << "\n";
    }
}

void LibrarySystem::ListUsers() const {
    for (const User* user : users_.GetAllUsers()) {
        std::cout << user->GetRegistrationNumber() << " | "
            << user->FullName() << " | "
            << user->RoleText() << " | "
            << user->GetUsername() << "\n";
    }
}

void LibrarySystem::RemoveUserInteractive(const User& signedInAdmin) {
    const std::string username = ReadLine("Username to remove: ");
    if (username == signedInAdmin.GetUsername()) {
        std::cout << "The active administrator account cannot remove itself.\n";
        return;
    }

    if (users_.RemoveByUsername(username)) {
        std::cout << "User removed. Registration numbers are never reused.\n";
    }
    else {
        std::cout << "User not found.\n";
    }
}

int LibrarySystem::ReadInt(const std::string& prompt, int minValue, int maxValue) {
    while (true) {
        std::cout << prompt;
        int value;
        if (std::cin >> value && value >= minValue && value <= maxValue) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "Enter a number from " << minValue << " to " << maxValue << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

double LibrarySystem::ReadDouble(const std::string& prompt, double minValue) {
    while (true) {
        std::cout << prompt;
        double value;
        if (std::cin >> value && value >= minValue) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "Enter a number greater than or equal to " << minValue << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string LibrarySystem::ReadLine(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

UserRole LibrarySystem::ReadRole() {
    std::cout << "1. Patron (00)\n"
        << "2. Administrator (01)\n"
        << "3. Librarian (02)\n"
        << "4. Manager (03)\n";
    const int choice = ReadInt("Role: ", 1, 4);

    switch (choice) {
    case 1: return UserRole::Patron;
    case 2: return UserRole::Admin;
    case 3: return UserRole::Librarian;
    case 4: return UserRole::Manager;
    }
    return UserRole::Patron;
}
void LibrarySystem::ShowcaseMode()
{
    std::cout << "\n";
    std::cout << "=====================================================\n";
    std::cout << "       SMART LIBRARY - COMPLETE FEATURE SHOWCASE\n";
    std::cout << "=====================================================\n";

    // -------------------------------------------------
    // PATRON FEATURES
    // -------------------------------------------------
    std::cout << "\n[PATRON FEATURES]\n";
    std::cout << "-----------------------------------------------------\n";
    std::cout << "1. Search Book Inventory\n";
    std::cout << "2. View Book Information\n";
    std::cout << "3. View Book Location\n";
    std::cout << "4. View My Account\n";
    std::cout << "5. Edit Payment Information\n";

    /*
    std::cout << "6. Check Out Book\n";
    std::cout << "7. Return Book\n";
    std::cout << "8. View Current Checkouts\n";
    std::cout << "9. View Due Dates\n";
    std::cout << "10. View Checkout History\n";
    std::cout << "11. Pay Overdue Fees\n";
    std::cout << "12. Purchase Eligible Overdue Book\n";
    std::cout << "13. View Book Recommendations\n";
    std::cout << "14. Reserve Book\n";
    */


    // -------------------------------------------------
    // LIBRARIAN FEATURES
    // -------------------------------------------------
    std::cout << "\n\n[LIBRARIAN / EMPLOYEE FEATURES]\n";
    std::cout << "-----------------------------------------------------\n";

    std::cout << "Includes all Patron permissions plus:\n\n";

    std::cout << "1. Add Book\n";
    std::cout << "2. Remove Book\n";
    std::cout << "3. Search Entire Inventory\n";
    std::cout << "4. Display All Books\n";
    std::cout << "5. View Book Locations\n";
    std::cout << "6. View Inventory Summary\n";
    std::cout << "7. Sort Books by Title\n";
    std::cout << "8. Sort Books by Author\n";
    std::cout << "9. Sort Books by Publication Year\n";

   /* 
    std::cout << "10. Check Book In\n";
    std::cout << "11. Check Book Out for Patron\n";
    std::cout << "12. View All Checked-Out Books\n";
    std::cout << "13. View Missing Books\n";
    std::cout << "14. Search Patrons with Overdue Books\n";
    std::cout << "15. Process Book Return\n";
    std::cout << "16. Restore Missing Book to Inventory\n";
    std::cout << "17. Restore Purchased Book to Inventory\n";
    std::cout << "18. Process / Void Eligible Patron Payment\n";
    */

    // -------------------------------------------------
    // MANAGER FEATURES
    // -------------------------------------------------
    std::cout << "\n\n[MANAGER FEATURES]\n";
    std::cout << "-----------------------------------------------------\n";

    std::cout << "Includes Patron and Librarian permissions.\n";
    std::cout << "Manager role code: 03\nThere are no current Manager Roles assigned.";

    // -------------------------------------------------
    // ADMIN FEATURES
    // -------------------------------------------------
    std::cout << "\n\n[ADMINISTRATOR FEATURES]\n";
    std::cout << "-----------------------------------------------------\n";

    std::cout << "1. Add User Account\n";
    std::cout << "2. Remove User Account\n";
    std::cout << "3. Search User Account\n";
    std::cout << "4. Display All Users\n";
    std::cout << "5. View User Registration Numbers\n";
    std::cout << "6. View User Roles\n";
    std::cout << "7. View System Inventory Information\n";

    // -------------------------------------------------
    // DATA STRUCTURES
    // -------------------------------------------------
   /* std::cout << "\n\n[DATA STRUCTURES / ALGORITHMS]\n";
    std::cout << "-----------------------------------------------------\n";

    std::cout << "- Hash Table: ISBN lookup\n";
    std::cout << "- Hash Table: Title lookup\n";
    std::cout << "- Linked List: User accounts\n";
    std::cout << "- Linear Search: Partial book titles\n";
    std::cout << "- Merge Sort: Book titles\n";
    std::cout << "- Quick Sort: Authors\n";
    std::cout << "- Heap Sort: Publication years\n";

    std::cout << "\nPlanned Advanced Structures:\n";
    std::cout << "- Stack: User activity history\n";
    std::cout << "- Queue: Book reservations\n";
    std::cout << "- Priority Queue / Heap: Due dates and overdue books\n";
    std::cout << "- Binary Search Tree: Recommendations\n";
    std::cout << "- Graph: Related books and user preferences\n";
    std::cout << "- Huffman Coding: Data compression\n";
    std::cout << "- Password Hashing: Account security\n";
    */

    // -------------------------------------------------
    // PHYSICAL LIBRARY
    // -------------------------------------------------
    std::cout << "\n\n[PHYSICAL LIBRARY CONFIGURATION]\n";
    std::cout << "-----------------------------------------------------\n";

    std::cout << "Quadrants:             10\n";
    std::cout << "Racks per Quadrant:     5\n";
    std::cout << "Books per Rack:       100\n";
    std::cout << "Total Shelf Capacity: 5000\n";

    std::cout << "\n=====================================================\n";
    std::cout << "                 END OF SHOWCASE\n";
    std::cout << "=====================================================\n";

    std::cout << "\nPress ENTER to return to the main menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
