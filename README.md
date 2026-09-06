# Smart Library Management System - Module 4 Core Build

C++17 console implementation for Modules 3 and 4 of the Smart Library Management System project.

## Implemented in this build

- Custom separate-chaining hash table for exact ISBN lookup.
- Second hash-table index for exact title lookup.
- Linear substring title search to demonstrate why exact hashing and partial search are different access patterns.
- 5,000 generated demo books with author, ISBN, title, publisher, year, illustrator, retail price, status, and shelf location.
- 10 quadrants x 5 racks x 100 slots = 5,000 physical shelf positions.
- Dynamic catalog: additional books can still be added after shelf capacity is reached, but they are marked `Unassigned / storage` until a slot opens.
- Custom singly linked list for user account management.
- System-wide increasing registration number; deleted registration numbers are never reused.
- Username generation: `Last.First<registrationNumber><roleCode>`.
  - Patron `00`
  - Administrator `01`
  - Librarian `02`
  - Manager `03`
- Role-based menus.
- Patron self-account view and payment-display update.
- Employees inherit patron capabilities and can add/remove/search inventory and see book locations.
- Administrators manage users and inspect inventory summaries.
- Merge sort by title, quicksort by author, and heap sort by publication year.

## Intentionally deferred to Module 5+

- Checkout and return transactions.
- Checkout history and current due dates.
- Overdue calculations and forced payment screen.
- Missing-book workflow.
- Purchase-at-half-retail workflow.
- Reservation queue.
- Deadline priority queue / overdue heap.
- Recommendation BST and graph.
- Activity-log stack.
- Huffman compression.
- Password hashing / secure credential storage.

These features are deferred because the supplied course outline places borrowing/returns, queues, heaps, priority queues, trees, and graphs after the Module 3/4 core implementation. Security should use a proper salted password KDF rather than `std::hash` or a raw SHA hash.

## Build with Visual Studio / CMake

Open the repository folder in Visual Studio 2022. Visual Studio should detect `CMakeLists.txt` automatically.

Command-line alternative:

```bash
cmake -S . -B build
cmake --build build --config Debug
```

On Linux/macOS:

```bash
./build/smart_library
```

On Windows with a multi-config generator, the executable is typically under:

```text
build/Debug/smart_library.exe
```

## Demo accounts

The program prints all seeded demo usernames at startup. The generated format is based on registration order, for example:

- `System.Admin101` = registration 1 + administrator code `01`
- `Houston.Marcus200` = registration 2 + patron code `00`
- `Morgan.Leah302` = registration 3 + librarian code `02`
- `Brooks.Daniel403` = registration 4 + manager code `03`

If Marcus Houston were the 300th registered patron, the generated username would be exactly:

```text
Houston.Marcus30000
```

## Design note: shelf capacity vs. unlimited additions

The requested physical layout has exactly 5,000 shelf slots. That conflicts with unlimited physical shelving. This implementation keeps the **catalog dynamic** while respecting physical capacity: once all 5,000 positions are occupied, newly added books are stored as `Unassigned / storage`. When a shelved book is removed, its location becomes available again.
