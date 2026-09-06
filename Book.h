#pragma once

#include <string>

enum class BookStatus {
    Available,
    CheckedOut,
    Missing,
    Purchased
};

struct BookLocation {
    bool assigned = false;
    int quadrant = 0;       // 1-10
    int rack = 0;           // 1-5 within the quadrant
    int slot = 0;           // 1-100 within the rack

    std::string ToString() const;
};

class Book {
public:
    Book(int catalogId,
        std::string isbn,
        std::string title,
        std::string author,
        std::string publisher,
        int publicationYear,
        std::string illustrator,
        double retailPrice,
        BookLocation location);

    int GetCatalogId() const;
    const std::string& GetIsbn() const;
    const std::string& GetTitle() const;
    const std::string& GetAuthor() const;
    const std::string& GetPublisher() const;
    int GetPublicationYear() const;
    const std::string& GetIllustrator() const;
    double GetRetailPrice() const;
    const BookLocation& GetLocation() const;
    BookStatus GetStatus() const;

    void SetStatus(BookStatus status);
    void SetLocation(const BookLocation& location);

    bool IsAvailable() const;
    std::string StatusText() const;

private:
    int catalogId_;
    std::string isbn_;
    std::string title_;
    std::string author_;
    std::string publisher_;
    int publicationYear_;
    std::string illustrator_;
    double retailPrice_;
    BookLocation location_;
    BookStatus status_ = BookStatus::Available;
};

