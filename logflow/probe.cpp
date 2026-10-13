#include <iostream>
#include <thread>
#include <version>

#if _MSVC_LANG < 202002L
#error "LogFlow needs C++20. Add /std:c++20 to the compile command."
#endif

int main() {
    int failures = 0;

    std::cout << "compiler version (_MSC_VER): " << _MSC_VER << "\n";
    std::cout << "language standard (_MSVC_LANG): " << _MSVC_LANG << "\n";
    std::cout << "standard library update (_MSVC_STL_UPDATE): " << _MSVC_STL_UPDATE << "\n";

    unsigned int cores = std::thread::hardware_concurrency();
    std::cout << "hardware threads: " << cores << "\n";

    const char* sanitizer = "none";
#ifdef __SANITIZE_ADDRESS__
    sanitizer = "AddressSanitizer";
#endif
    std::cout << "sanitizer in this build: " << sanitizer << "\n";

    bool has_jthread = false;
#ifdef __cpp_lib_jthread
    has_jthread = true;
#endif

    bool has_span = false;
#ifdef __cpp_lib_span
    has_span = true;
#endif

    if (_MSC_VER < 1930) {
        std::cout << "[FAIL] Visual Studio 2022 compiler (MSVC 19.30 or newer)\n";
        ++failures;
    }
    if (!has_jthread) {
        std::cout << "[FAIL] std::jthread and stop_token\n";
        ++failures;
    }
    if (!has_span) {
        std::cout << "[FAIL] std::span\n";
        ++failures;
    }
    if (cores < 2) {
        std::cout << "[FAIL] at least 2 hardware threads\n";
        ++failures;
    }

    if (failures == 0) {
        std::cout << "READY: all required checks passed\n";
    }
    return failures;
}
