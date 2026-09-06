#pragma once

#include "BookHashTable.h"
#include "Book.h"

#include <array>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

class BookInventory {
public:
    BookInventory();

    void GenerateDemoBooks(std::size_t count = 5000);

    Book* AddBook(const std::string& isbn,
        const std::string& title,
        const std::string& author,
        const std::string& publisher,
        int publicationYear,
        const std::string& illustrator,
        double retailPrice);

    bool RemoveBookByIsbn(const std::string& isbn);

    Book* FindByIsbn(const std::string& isbn) const;
    std::vector<Book*> FindByExactTitle(const std::string& title) const;
    std::vector<Book*> SearchTitleContains(const std::string& text) const;

    std::vector<Book*> GetBooksSortedByTitle() const;
    std::vector<Book*> GetBooksSortedByAuthor() const;
    std::vector<Book*> GetBooksSortedByYear() const;

    std::size_t TotalBooks() const;
    std::size_t AvailableBooks() const;
    std::size_t UnavailableBooks() const;

    double IsbnLoadFactor() const;
    double TitleLoadFactor() const;

private:
    static constexpr int kQuadrants = 10;
    static constexpr int kRacksPerQuadrant = 5;
    static constexpr int kSlotsPerRack = 100;
    static constexpr int kShelfCapacity =
        kQuadrants * kRacksPerQuadrant * kSlotsPerRack;

    std::vector<std::unique_ptr<Book>> books_;
    BookHashTable isbnIndex_;
    BookHashTable titleIndex_;
    int nextCatalogId_ = 1;
    std::array<bool, kShelfCapacity> occupiedSlots_{};

    BookLocation AllocateOpenLocation();
    void ReleaseLocation(const BookLocation& location);

    static std::string Lower(std::string value);
    static std::string MakeDemoIsbn(int index);

    static void MergeSortByTitle(std::vector<Book*>& books);
    static void MergeSortByTitle(std::vector<Book*>& books,
        std::vector<Book*>& temp,
        int left,
        int right);
    static void MergeTitleRanges(std::vector<Book*>& books,
        std::vector<Book*>& temp,
        int left,
        int middle,
        int right);

    static void QuickSortByAuthor(std::vector<Book*>& books, int low, int high);

    static void HeapSortByYear(std::vector<Book*>& books);
    static void HeapifyByYear(std::vector<Book*>& books, int heapSize, int root);
};
