# Annotated Reader Diff: batch_reader.h

Lines starting with `-` were removed, lines starting with `+` were added.

```diff
@@ -1,17 +1,15 @@
 #pragma once
 
 #include <cstddef>
-#include <cstdio>
+#include <fstream>
 #include <string>
+#include <vector>
 
 #include "log_parser.h"
 
 class BatchReader {
 public:
     explicit BatchReader(const std::string& path);
-    ~BatchReader();
-    BatchReader(const BatchReader&) = delete;
-    BatchReader& operator=(const BatchReader&) = delete;
 
     bool is_open() const;
     bool next(LogRecord& record);
@@ -19,12 +17,8 @@
     std::size_t rejected_at(std::size_t index) const;
 
 private:
-    void remember_rejected(std::size_t line_number);
-
-    std::FILE* file_;
-    char* buffer_;
-    std::size_t line_number_;
-    std::size_t* rejected_;
-    std::size_t rejected_count_;
-    std::size_t rejected_capacity_;
+    std::ifstream in_;
+    std::string line_;
+    std::size_t line_number_ = 0;
+    std::vector<std::size_t> rejected_;
 };
```

- The destructor and the two deleted copy operations are gone. Nothing needs them any more.
- `std::ifstream`, `std::string` and `std::vector` each release their own resources.
- `remember_rejected` and the two bookkeeping counters are gone. `push_back` replaces all three.

# Annotated Reader Diff: batch_reader.cpp

```diff
@@ -1,48 +1,25 @@
 #include "batch_reader.h"
 
-#include <cstring>
 #include <optional>
 
-namespace {
-constexpr std::size_t kBufferSize = 512;
-}
-
-BatchReader::BatchReader(const std::string& path)
-    : file_(std::fopen(path.c_str(), "r")),
-      buffer_(new char[kBufferSize]),
-      line_number_(0),
-      rejected_(nullptr),
-      rejected_count_(0),
-      rejected_capacity_(0) {}
-
-BatchReader::~BatchReader() {
-    delete[] rejected_;
-    delete[] buffer_;
-    if (file_ != nullptr) {
-        std::fclose(file_);
-    }
-}
+BatchReader::BatchReader(const std::string& path) : in_(path) {}
 
 bool BatchReader::is_open() const {
-    return file_ != nullptr;
+    return in_.is_open();
 }
 
 bool BatchReader::next(LogRecord& record) {
-    if (file_ == nullptr) {
-        return false;
-    }
-    while (std::fgets(buffer_, kBufferSize, file_) != nullptr) {
+    while (std::getline(in_, line_)) {
         ++line_number_;
-        std::size_t length = std::strlen(buffer_);
-        while (length > 0 && (buffer_[length - 1] == '\n' || buffer_[length - 1] == '\r')) {
-            buffer_[--length] = '\0';
+        if (!line_.empty() && line_.back() == '\r') {
+            line_.pop_back();
         }
-        if (length == 0) {
+        if (line_.empty()) {
             continue;
         }
-        std::optional<LogRecord> parsed = parse_line(buffer_);
+        std::optional<LogRecord> parsed = parse_line(line_);
         if (!parsed) {
-            remember_rejected(line_number_);
+            rejected_.push_back(line_number_);
             continue;
         }
         record = *parsed;
@@ -52,23 +29,9 @@
 }
 
 std::size_t BatchReader::rejected_count() const {
-    return rejected_count_;
+    return rejected_.size();
 }
 
 std::size_t BatchReader::rejected_at(std::size_t index) const {
     return rejected_[index];
 }
-
-void BatchReader::remember_rejected(std::size_t line_number) {
-    if (rejected_count_ == rejected_capacity_) {
-        std::size_t new_capacity = rejected_capacity_ == 0 ? 4 : rejected_capacity_ * 2;
-        std::size_t* grown = new std::size_t[new_capacity];
-        for (std::size_t i = 0; i < rejected_count_; ++i) {
-            grown[i] = rejected_[i];
-        }
-        delete[] rejected_;
-        rejected_ = grown;
-        rejected_capacity_ = new_capacity;
-    }
-    rejected_[rejected_count_++] = line_number;
-}
```

- The constructor shrinks to one initializer.
- `next` reads with `std::getline` into a string, so the 512-character limit disappears.
- The growth loop becomes one `push_back` call.
