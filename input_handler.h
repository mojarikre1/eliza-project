// ============================================================
// input_handler.h
// Declares functions for reading and normalizing user input.
// ============================================================

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <string>

// Converts the raw user string to lowercase and trims leading/trailing whitespace.
// Returns the normalized string.
std::string normalizeInput(const std::string& raw);

#endif // INPUT_HANDLER_H
