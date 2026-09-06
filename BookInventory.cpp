#include "BookInventory.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <iomanip>
#include <random>
#include <sstream>
#include <stdexcept>
#include <utility>

BookInventory::BookInventory()
    : isbnIndex_(10007), titleIndex_(10007) {
}

void BookInventory::GenerateDemoBooks(std::size_t count) {
    if (!books_.empty()) {
        return;
    }

    static const std::array<std::string, 20> adjectives = {
        "Silent", "Hidden", "Golden", "Ancient", "Electric",
        "Crimson", "Northern", "Fading", "Infinite", "Last",
        "Secret", "Broken", "Brilliant", "Distant", "Midnight",
        "Forgotten", "Silver", "Wandering", "Parallel", "Emerald"
    };

    static const std::array<std::string, 20> nouns = {
        "Orbit", "Harbor", "Archive", "River", "Machine",
        "Garden", "Signal", "Voyage", "Library", "Kingdom",
        "Equation", "Mountain", "Lantern", "Chronicle", "Bridge",
        "Compass", "Circuit", "Forest", "Horizon", "Code"
    };

    static const std::array<std::string, 16> firstNames = {
        "Avery", "Morgan", "Jordan", "Taylor", "Reese", "Cameron",
        "Casey", "Riley", "Parker", "Quinn", "Hayden", "Emerson",
        "Logan", "Rowan", "Drew", "Skyler"
    };

    static const std::array<std::string, 16> lastNames = {
        "Bennett", "Carter", "Diaz", "Ellis", "Foster", "Gray",
        "Hayes", "Irwin", "Johnson", "King", "Lewis", "Morris",
        "Nguyen", "Owens", "Patel", "Reed"
    };

    static const std::array<std::string, 10> publishers = {
        "Northstar Press", "Pioneer House", "Cedar Books", "Blue Arc Publishing",
        "Keystone Editions", "Summit Press", "Redwood House", "Atlas Works",
        "Harborline Publishing", "Prairie Press"
    };

    // Fixed seed gives a randomized-looking 5,000-book data set that is
    std::mt19937 rng(3622026);
    std::uniform_int_distribution<int> adjectivePick(0, static_cast<int>(adjectives.size()) - 1);
    std::uniform_int_distribution<int> nounPick(0, static_cast<int>(nouns.size()) - 1);
    std::uniform_int_distribution<int> namePick(0, static_cast<int>(firstNames.size()) - 1);
    std::uniform_int_distribution<int> lastPick(0, static_cast<int>(lastNames.size()) - 1);
    std::uniform_int_distribution<int> publisherPick(0, static_cast<int>(publishers.size()) - 1);
    std::uniform_int_distribution<int> yearPick(1950, 2026);
    std::uniform_int_distribution<int> centsPick(899, 5999);
    std::uniform_int_distribution<int> illustratorChance(0, 3);

    for (std::size_t i = 1; i <= count; ++i) {
        const std::string title =
            "The " + adjectives[adjectivePick(rng)] + " " + nouns[nounPick(rng)] +
            " #" + std::to_string(i);
        const std::string author =
            firstNames[namePick(rng)] + " " + lastNames[lastPick(rng)];
        const std::string illustrator = illustratorChance(rng) == 0
            ? firstNames[namePick(rng)] + " " + lastNames[lastPick(rng)]
            : "N/A";

        AddBook(MakeDemoIsbn(static_cast<int>(i)),
            title,
            author,
            publishers[publisherPick(rng)],
            yearPick(rng),
            illustrator,
            static_cast<double>(centsPick(rng)) / 100.0);
    }
}

Book* BookInventory::AddBook(const std::string& isbn,
    const std::string& title,
    const std::string& author,
    const std::string& publisher,
    int publicationYear,
    const std::string& illustrator,
    double retailPrice) {
    if (isbn.empty() || title.empty() || author.empty() || publisher.empty()) {
        throw std::invalid_argument("ISBN, title, author, and publisher are required.");
    }
    if (FindByIsbn(isbn) != nullptr) {
        throw std::invalid_argument("A book with that ISBN already exists.");
    }
    if (publicationYear < 0 || publicationYear > 2100) {
        throw std::invalid_argument("Publication year is outside the accepted range.");
    }
    if (retailPrice < 0.0) {
        throw std::invalid_argument("Retail price cannot be negative.");
    }

    auto book = std::make_unique<Book>(
        nextCatalogId_++, isbn, title, author, publisher,
        publicationYear, illustrator.empty() ? "N/A" : illustrator,
        retailPrice, AllocateOpenLocation());

    Book* raw = book.get();
    books_.push_back(std::move(book));
    isbnIndex_.Insert(raw->GetIsbn(), raw);
    titleIndex_.Insert(raw->GetTitle(), raw);
    return raw;
}

bool BookInventory::RemoveBookByIsbn(const std::string& isbn) {
    Book* target = FindByIsbn(isbn);
    if (target == nullptr) {
        return false;
    }

    isbnIndex_.Remove(target->GetIsbn(), target);
    titleIndex_.Remove(target->GetTitle(), target);
    ReleaseLocation(target->GetLocation());

    books_.erase(
        std::remove_if(books_.begin(), books_.end(),
            [&](const std::unique_ptr<Book>& book) {
                return book.get() == target;
            }),
        books_.end());
    return true;
}

Book* BookInventory::FindByIsbn(const std::string& isbn) const {
    return isbnIndex_.FindFirst(isbn);
}

std::vector<Book*> BookInventory::FindByExactTitle(const std::string& title) const {
    return titleIndex_.FindAll(title);
}

std::vector<Book*> BookInventory::SearchTitleContains(const std::string& text) const {
    const std::string needle = Lower(text);
    std::vector<Book*> matches;

    // Hash tables excel at exact lookup.  Substring lookup requires a scan.

    for (const auto& book : books_) {
        if (Lower(book->GetTitle()).find(needle) != std::string::npos) {
            matches.push_back(book.get());
        }
    }
    return matches;
}

std::vector<Book*> BookInventory::GetBooksSortedByTitle() const {
    std::vector<Book*> result;
    result.reserve(books_.size());
    for (const auto& book : books_) result.push_back(book.get());
    MergeSortByTitle(result);
    return result;
}

std::vector<Book*> BookInventory::GetBooksSortedByAuthor() const {
    std::vector<Book*> result;
    result.reserve(books_.size());
    for (const auto& book : books_) result.push_back(book.get());
    if (!result.empty()) {
        QuickSortByAuthor(result, 0, static_cast<int>(result.size()) - 1);
    }
    return result;
}

std::vector<Book*> BookInventory::GetBooksSortedByYear() const {
    std::vector<Book*> result;
    result.reserve(books_.size());
    for (const auto& book : books_) result.push_back(book.get());
    HeapSortByYear(result);
    return result;
}

std::size_t BookInventory::TotalBooks() const { return books_.size(); }

std::size_t BookInventory::AvailableBooks() const {
    return static_cast<std::size_t>(
        std::count_if(books_.begin(), books_.end(),
            [](const std::unique_ptr<Book>& book) {
                return book->IsAvailable();
            }));
}

std::size_t BookInventory::UnavailableBooks() const {
    return TotalBooks() - AvailableBooks();
}

double BookInventory::IsbnLoadFactor() const { return isbnIndex_.LoadFactor(); }
double BookInventory::TitleLoadFactor() const { return titleIndex_.LoadFactor(); }

BookLocation BookInventory::AllocateOpenLocation() {
    for (int index = 0; index < kShelfCapacity; ++index) {
        if (!occupiedSlots_[static_cast<std::size_t>(index)]) {
            occupiedSlots_[static_cast<std::size_t>(index)] = true;

            const int rackGlobal = index / kSlotsPerRack;
            const int slot = index % kSlotsPerRack + 1;
            const int quadrant = rackGlobal / kRacksPerQuadrant + 1;
            const int rack = rackGlobal % kRacksPerQuadrant + 1;
            return { true, quadrant, rack, slot };
        }
    }


    return {};
}

void BookInventory::ReleaseLocation(const BookLocation& location) {
    if (!location.assigned) return;

    const int rackGlobal =
        (location.quadrant - 1) * kRacksPerQuadrant + (location.rack - 1);
    const int index = rackGlobal * kSlotsPerRack + (location.slot - 1);

    if (index >= 0 && index < kShelfCapacity) {
        occupiedSlots_[static_cast<std::size_t>(index)] = false;
    }
}

std::string BookInventory::Lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return value;
}

std::string BookInventory::MakeDemoIsbn(int index) {
    std::ostringstream body;
    body << "978" << std::setfill('0') << std::setw(9) << index;
    const std::string firstTwelve = body.str();

    int sum = 0;
    for (std::size_t i = 0; i < firstTwelve.size(); ++i) {
        const int digit = firstTwelve[i] - '0';
        sum += (i % 2 == 0) ? digit : 3 * digit;
    }
    const int checkDigit = (10 - (sum % 10)) % 10;
    return firstTwelve + static_cast<char>('0' + checkDigit);
}

void BookInventory::MergeSortByTitle(std::vector<Book*>& books) {
    if (books.size() < 2) return;
    std::vector<Book*> temp(books.size());
    MergeSortByTitle(books, temp, 0, static_cast<int>(books.size()) - 1);
}

void BookInventory::MergeSortByTitle(std::vector<Book*>& books,
    std::vector<Book*>& temp,
    int left,
    int right) {
    if (left >= right) return;
    const int middle = left + (right - left) / 2;
    MergeSortByTitle(books, temp, left, middle);
    MergeSortByTitle(books, temp, middle + 1, right);
    MergeTitleRanges(books, temp, left, middle, right);
}

void BookInventory::MergeTitleRanges(std::vector<Book*>& books,
    std::vector<Book*>& temp,
    int left,
    int middle,
    int right) {
    int i = left;
    int j = middle + 1;
    int k = left;

    while (i <= middle && j <= right) {
        if (Lower(books[i]->GetTitle()) <= Lower(books[j]->GetTitle())) {
            temp[k++] = books[i++];
        }
        else {
            temp[k++] = books[j++];
        }
    }
    while (i <= middle) temp[k++] = books[i++];
    while (j <= right) temp[k++] = books[j++];
    for (int index = left; index <= right; ++index) books[index] = temp[index];
}

void BookInventory::QuickSortByAuthor(std::vector<Book*>& books, int low, int high) {
    if (low >= high) return;

    // Three-way quicksort handles duplicate author names efficiently.
    const std::string pivot = Lower(books[low + (high - low) / 2]->GetAuthor());
    int less = low;
    int current = low;
    int greater = high;

    while (current <= greater) {
        const std::string value = Lower(books[current]->GetAuthor());
        if (value < pivot) {
            std::swap(books[less], books[current]);
            ++less;
            ++current;
        }
        else if (value > pivot) {
            std::swap(books[current], books[greater]);
            --greater;
        }
        else {
            ++current;
        }
    }

    QuickSortByAuthor(books, low, less - 1);
    QuickSortByAuthor(books, greater + 1, high);
}

void BookInventory::HeapifyByYear(std::vector<Book*>& books, int heapSize, int root) {
    int largest = root;
    const int left = 2 * root + 1;
    const int right = 2 * root + 2;

    if (left < heapSize &&
        books[left]->GetPublicationYear() > books[largest]->GetPublicationYear()) {
        largest = left;
    }
    if (right < heapSize &&
        books[right]->GetPublicationYear() > books[largest]->GetPublicationYear()) {
        largest = right;
    }
    if (largest != root) {
        std::swap(books[root], books[largest]);
        HeapifyByYear(books, heapSize, largest);
    }
}

void BookInventory::HeapSortByYear(std::vector<Book*>& books) {
    const int count = static_cast<int>(books.size());
    for (int i = count / 2 - 1; i >= 0; --i) {
        HeapifyByYear(books, count, i);
    }
    for (int end = count - 1; end > 0; --end) {
        std::swap(books[0], books[end]);
        HeapifyByYear(books, end, 0);
    }
}
