#include "BookHashTable.h"

#include "Book.h"

#include <algorithm>
#include <cctype>
#include <functional>

BookHashTable::BookHashTable(std::size_t bucketCount)
    : buckets_(bucketCount) {
}

void BookHashTable::Insert(const std::string& key, Book* book) {
    const std::string normalized = Normalize(key);
    buckets_[BucketIndex(normalized)].push_back({ normalized, book });
    ++size_;
}

Book* BookHashTable::FindFirst(const std::string& key) const {
    const std::string normalized = Normalize(key);
    const auto& bucket = buckets_[BucketIndex(normalized)];

    for (const Entry& entry : bucket) {
        if (entry.key == normalized) {
            return entry.book;
        }
    }
    return nullptr;
}

std::vector<Book*> BookHashTable::FindAll(const std::string& key) const {
    const std::string normalized = Normalize(key);
    const auto& bucket = buckets_[BucketIndex(normalized)];

    std::vector<Book*> matches;
    for (const Entry& entry : bucket) {
        if (entry.key == normalized) {
            matches.push_back(entry.book);
        }
    }
    return matches;
}

bool BookHashTable::Remove(const std::string& key, const Book* book) {
    const std::string normalized = Normalize(key);
    auto& bucket = buckets_[BucketIndex(normalized)];

    const auto oldSize = bucket.size();
    bucket.erase(
        std::remove_if(bucket.begin(), bucket.end(),
            [&](const Entry& entry) {
                return entry.key == normalized && entry.book == book;
            }),
        bucket.end());

    if (bucket.size() != oldSize) {
        --size_;
        return true;
    }
    return false;
}

std::size_t BookHashTable::Size() const { return size_; }

double BookHashTable::LoadFactor() const {
    return buckets_.empty()
        ? 0.0
        : static_cast<double>(size_) / static_cast<double>(buckets_.size());
}

std::size_t BookHashTable::BucketIndex(const std::string& key) const {
    return std::hash<std::string>{}(key) % buckets_.size();
}

std::string BookHashTable::Normalize(const std::string& key) {
    std::string normalized;
    normalized.reserve(key.size());

    for (char ch : key) {
        normalized.push_back(
            static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
    }
    return normalized;
}
