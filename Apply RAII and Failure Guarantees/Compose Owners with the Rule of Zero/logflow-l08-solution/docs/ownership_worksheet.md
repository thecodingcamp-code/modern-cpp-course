# Ownership Worksheet: BatchReader

## Members before the refactor

| Member | What it owns | Who cleans it up? | Standard replacement |
|---|---|---|---|
| `std::FILE* file_` | an open file | | |
| `char* buffer_` | a 512-character array | | |
| `std::size_t* rejected_` | a growing array of line numbers | | |
| `rejected_count_`, `rejected_capacity_` | bookkeeping for that array | | |

## Special members before the refactor

| Special member | Why was it written? | Still needed after the refactor? |
|---|---|---|
| `~BatchReader()` | | |
| `BatchReader(const BatchReader&) = delete` | | |
| `BatchReader& operator=(const BatchReader&) = delete` | | |
| `remember_rejected` (grows the array by hand) | | |

## Answer key

| Member | Who cleans it up? | Standard replacement |
|---|---|---|
| `file_` | the destructor, with `fclose` | `std::ifstream in_` |
| `buffer_` | the destructor, with `delete[]` | `std::string line_` |
| `rejected_` | the destructor, with `delete[]`, and `remember_rejected` on growth | `std::vector<std::size_t> rejected_` |
| `rejected_count_`, `rejected_capacity_` | nobody, but they must stay in sync with the array | not needed, the vector tracks its own size |

| Special member | Why was it written? | Needed after? |
|---|---|---|
| destructor | to release the file and both arrays | no, each member releases itself |
| deleted copy constructor | to stop two readers freeing the same memory | no, the members already refuse to be copied or copy safely |
| deleted copy assignment | same reason | no |
| `remember_rejected` | to grow the array by hand | no, `push_back` does it |

The Rule of Zero: if every member already owns itself, the class declares none of the special members, and the compiler-written ones are correct.
