#include "Book.h"

#include <sstream>
#include <utility>

std::string BookLocation::ToString() const {
    if (!assigned) {
        return "Unassigned / storage";
    }

    std::ostringstream out;
    out << "Quadrant " << quadrant
        << ", Rack " << rack
        << ", Slot " << slot;
    return out.str();
}

Book::Book(int catalogId,
    std::string isbn,
    std::string title,
    std::string author,
    std::string publisher,
    int publicationYear,
    std::string illustrator,
    double retailPrice,
    BookLocation location)
    : catalogId_(catalogId),
    isbn_(std::move(isbn)),
    title_(std::move(title)),
    author_(std::move(author)),
    publisher_(std::move(publisher)),
    publicationYear_(publicationYear),
    illustrator_(std::move(illustrator)),
    retailPrice_(retailPrice),
    location_(location) {
}

int Book::GetCatalogId() const { return catalogId_; }
const std::string& Book::GetIsbn() const { return isbn_; }
const std::string& Book::GetTitle() const { return title_; }
const std::string& Book::GetAuthor() const { return author_; }
const std::string& Book::GetPublisher() const { return publisher_; }
int Book::GetPublicationYear() const { return publicationYear_; }
const std::string& Book::GetIllustrator() const { return illustrator_; }
double Book::GetRetailPrice() const { return retailPrice_; }
const BookLocation& Book::GetLocation() const { return location_; }
BookStatus Book::GetStatus() const { return status_; }

void Book::SetStatus(BookStatus status) { status_ = status; }
void Book::SetLocation(const BookLocation& location) { location_ = location; }

bool Book::IsAvailable() const {
    return status_ == BookStatus::Available;
}

std::string Book::StatusText() const {
    switch (status_) {
    case BookStatus::Available: return "Available";
    case BookStatus::CheckedOut: return "Checked out";
    case BookStatus::Missing: return "Missing";
    case BookStatus::Purchased: return "Purchased";
    }
    return "Unknown";
}
