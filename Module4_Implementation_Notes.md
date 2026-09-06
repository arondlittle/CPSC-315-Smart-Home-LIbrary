# Module 3 & 4 - Core Feature Implementation Notes

## Requirement mapping

| Requirement | Module 4 implementation |
|---|---|
| Book inventory uses hash tables | `BookHashTable` is a custom separate-chaining hash table. `BookInventory` creates one ISBN index and one title index. |
| Search books quickly | Exact ISBN/title searches use hash lookup. Partial-title search is intentionally a linear scan because substring queries do not map directly to an exact hash key. |
| Sort books | Merge sort = title, quicksort = author, heap sort = publication year. |
| User accounts use linked lists | `UserLinkedList` is a custom singly linked list with dynamically allocated nodes. |
| 5,000 starting books | `GenerateDemoBooks(5000)` builds a repeatable pseudo-random synthetic catalog at startup. |
| Required book fields | Author, ISBN, title, publisher, publication year, illustrator, retail price, status, and physical location are stored. |
| 10 quadrants / 5 racks each / 100 books each | Inventory allocator provides exactly 5,000 shelf positions. |
| More catalog records can be added | The vector/hash indexes are dynamic. Once 5,000 shelf positions are occupied, added books receive `Unassigned / storage` until a slot becomes free. |
| User registration tally | Every added user gets the next registration number. Deleted registration numbers are not reused. |
| Username role suffix | Patron `00`, admin `01`, librarian `02`, manager `03`. |
| Example username | Registration 300 + patron role produces `Houston.Marcus30000`. |
| Login asks account type | User chooses Patron, Administrator, or Employee before entering the username. The stored role and username suffix must agree. |
| Different role access | Patron, employee, and administrator menus expose different operations. Librarian and manager are employees and also receive patron functions. |
| Employee inventory control | Librarian/manager can add, remove, search, locate, summarize, and sort books. |
| Patron self-service | Patron can view own account, update masked payment display data, and search/browse books. |
| Administrator account management | Administrator can add/search/list/remove user accounts and inspect inventory. |

## Features intentionally not implemented yet

The following ideas are valid for the complete project but should **not** be part of the Module 3/4 deliverable because the course outline assigns their core data structures/algorithms to later modules:

- Checkout and return processing.
- Checkout history and due dates.
- Overdue fee calculations and forced overdue-payment login flow.
- Missing-book tracking.
- Buying a book for half its retail value.
- Voiding return/payment transactions.
- Reservation queue.
- Activity log stack.
- Due-date heap and overdue priority queue.
- Recommendation binary search tree and graph.
- Huffman compression.
- Password security implementation.

The `BookStatus` enum already contains `Available`, `CheckedOut`, `Missing`, and `Purchased` so Module 5 can change status without replacing the book model.

## Design decisions where requirements conflict

### 5,000 physical slots versus unlimited additions

Ten quadrants x five racks x one hundred books equals exactly 5,000 physical positions. Unlimited new *shelved* books would violate that capacity. The program therefore allows unlimited catalog growth but marks overflow books as `Unassigned / storage` until a shelf position opens.

### Payment information

The Module 4 program stores only the last four display digits. It does not store a complete card number, CVV, or other sensitive payment credential. A classroom library program should not simulate unsafe storage of payment-card secrets.

### Password hashing

No password format was specified in the UML description. A later security implementation should use a salted password KDF such as Argon2id, bcrypt, scrypt, or PBKDF2. `std::hash` and unsalted SHA hashing should not be presented as password security.

## Module 5 extension points

The current design can be extended without changing the core data structures:

- Add `Loan`, `Reservation`, and `ActivityLogEntry` models.
- Add a queue of reservations.
- Add a min-heap/priority queue keyed by due date.
- Add checkout history to user records.
- Change `BookStatus` during checkout, return, missing, and purchase operations.
- Compute `AvailableBooks()` from book status automatically.
- Add BST/graph recommendation services beside `BookInventory` and `UserLinkedList`.
