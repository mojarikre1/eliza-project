// ============================================================
// input_handler.cpp
// Implements user-input normalization.
// ============================================================

#include "input_handler.h"
#include <algorithm>
#include <cctype>

// normalizeInput:
//   1. Converts every character to lowercase.
//   2. Strips leading whitespace.
//   3. Strips trailing whitespace.
// This ensures pattern matching is case-insensitive and robust
// to accidental spacing.
std::string normalizeInput(const std::string& raw) {
    std::string result = raw;

    // Convert to lowercase
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    // Trim leading whitespace
    size_t start = result.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return ""; // entirely whitespace
    }
    result = result.substr(start);

    // Trim trailing whitespace
    size_t end = result.find_last_not_of(" \t\r\n");
    result = result.substr(0, end + 1);

    return result;
}
