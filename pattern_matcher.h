// ============================================================
// pattern_matcher.h
// Declares functions for matching user input to one of the
// major category patterns using C++ regular expressions.
// ============================================================

#ifndef PATTERN_MATCHER_H
#define PATTERN_MATCHER_H

#include <string>

// Category identifiers returned by matchPattern().
// Each string maps to a response bank in response_generator.cpp.
static const std::string CAT_GREETING      = "greeting";
static const std::string CAT_GENERIC_VERB  = "generic_verb";
static const std::string CAT_LOVE          = "love";
static const std::string CAT_CONCERN       = "concern";
static const std::string CAT_FINANCIAL     = "financial";
static const std::string CAT_WELLBEING     = "wellbeing";
static const std::string CAT_EDUCATION     = "education";
static const std::string CAT_ENTERTAINMENT = "entertainment";
static const std::string CAT_FAMILY        = "family";
static const std::string CAT_WORK          = "work";
static const std::string CAT_NEGATION      = "negation";
static const std::string CAT_AFFIRMATION   = "affirmation";
static const std::string CAT_QUESTION      = "question";
static const std::string CAT_CATCHALL      = "catchall";

// Examines the normalized input string and returns the best-matching
// category constant (see above).  Uses regex_search() internally
// over a prioritized list of 50+ patterns.
std::string matchPattern(const std::string& input);

#endif // PATTERN_MATCHER_H
