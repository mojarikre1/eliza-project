// ============================================================
// response_generator.h
// Declares functions for generating ELIZA-style responses
// based on the matched pattern category and, optionally,
// content extracted via regex_replace() from the raw input.
// ============================================================

#ifndef RESPONSE_GENERATOR_H
#define RESPONSE_GENERATOR_H

#include <string>

// Selects and returns an appropriate psychiatrist-style response.
//
// Parameters:
//   category  - one of the CAT_* constants from pattern_matcher.h
//   input     - the normalized user input (used for pronoun
//               reflection and regex_replace() substitutions)
//
// Returns a ready-to-display response string.
std::string generateResponse(const std::string& category,
                             const std::string& input);

// Helper: performs pronoun reflection so that "I feel sad"
// becomes "you feel sad" in the echoed portion of a response.
// Uses regex_replace() on common first-person pronouns.
std::string reflectPronouns(const std::string& input);

#endif // RESPONSE_GENERATOR_H
