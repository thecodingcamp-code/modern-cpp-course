# RAII Sheet: Early-Exit Cleanup

## The rule

Acquire in the constructor. Release in the destructor. Then every way out of the scope releases for you.

## Before and after

| | Manual cleanup | RAII |
|---|---|---|
| Open | `file = tracked_open(path)` | `FileGuard file(path)` |
| Close | `tracked_close(file)` before every `return` and once at the end | nothing, the destructor does it |
| Add a new `return` later | you must remember to close first | nothing to remember |
| Forget one close | handle leaks, found only by a test or a tool | not possible |

## Refactor checklist

1. Count the exits. List every `return` after the resource is acquired.
2. Mark which exits close the resource and which do not. A mismatch is the leak.
3. Create an owner class: constructor acquires, destructor releases.
4. Delete the copy constructor and copy assignment, so two owners can never release the same handle.
5. Replace the raw handle with the owner. Use `.get()` where the old code passed the raw handle.
6. Delete every manual close call.
7. Rebuild and run the cleanup tests.

## Exit count for `analyze_legacy` (fill in before refactoring)

| Exit | Line is | Closes the file? |
|---|---|---|
| cannot open file | | |
| line too long | | |
| malformed line | | |
| normal end | | |

## Answer key

| Exit | Closes the file? |
|---|---|
| cannot open file | not needed, nothing was opened |
| line too long | yes |
| malformed line | **no, this is the leak** |
| normal end | yes |
