# Lifetime Trace Worksheet

## The lifetime of one file stream

```
main loop, one file
|
|  analyze_file(path)                  <- the job starts
|  |
|  |  TracedStream source(path)        <- ACQUIRE: owner created, file opened
|  |  |
|  |  |  read lines, count levels      <- the stream is usable here
|  |  |
|  |  |  trace: reached its return
|  |  |
|  |  return report                    <- scope ends
|  |  ~TracedStream()                  <- RELEASE: owner destroyed, file closed
|  |
|  back in main, next file starts      <- previous file is already closed
```

## Your task

Run LogFlow on the three sample files and copy the first three trace lines
into the table. For each line, write which line of code printed it and what
the owner is doing at that moment.

| Trace line | Printed by (file and line of code) | What the owner is doing |
|------------|------------------------------------|-------------------------|
|            |                                    |                         |
|            |                                    |                         |
|            |                                    |                         |

Then answer:

1. After `analyze_file` prints "reached its return", is the file still open?
2. Which line of code closes the file? (Hint: there is no `close()` call.)
3. Does the second file's "acquire" line appear before or after the first
   file's "release" line? Why?

## Answer key

Trace for the first file:

```
[trace] acquire data/sample/checkout-api.log
[trace] analyze_file reached its return
[trace] release data/sample/checkout-api.log
```

| Trace line | Printed by | What the owner is doing |
|------------|------------|-------------------------|
| acquire    | `TracedStream` constructor in `src/traced_stream.h` | Just created; the file stream inside it has opened the file |
| analyze_file reached its return | the last `std::cerr` line in `analyze_file` in `src/analyzer.cpp` | Still alive; the file is still open |
| release    | `~TracedStream()` destructor in `src/traced_stream.h` | Being destroyed because the scope of `analyze_file` ended |

1. Yes. The file is still open at that line. It closes after the function's
   scope ends, when the owner is destroyed.
2. The closing brace of `analyze_file`, or any earlier `return`, ends the scope
   of `source`. The compiler then calls the destructor, which releases the file.
3. After. The files are processed one at a time, so each owner is destroyed
   before the next one is created.
