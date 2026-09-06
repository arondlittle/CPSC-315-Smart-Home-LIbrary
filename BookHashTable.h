#pragma once

#include <cstddef>
#include <string>
#include <vector>

class Book;

// A separate-chaining hash table.  The inventory uses one instance for ISBN
// and a second instance for title, allowing both access patterns to be shown.
class BookHashTable {
public:
    explicit BookHashTable(std::size_t bucketCount = 10007);

    void Insert(const std::string& key, Book* book);
    Book* FindFirst(const std::string& key) const;
    std::vector<Book*> FindAll(const std::string& key) const;
    bool Remove(const std::string& key, const Book* book);

    std::size_t Size() const;
    double LoadFactor() const;

private:
    struct Entry {
        std::string key;
        Book* book = nullptr;
    };

    std::vector<std::vector<Entry>> buckets_;
    std::size_t size_ = 0;

    std::size_t BucketIndex(const std::string& key) const;
    static std::string Normalize(const std::string& key);
};

