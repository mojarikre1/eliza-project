// ============================================================
// pattern_matcher.cpp
// Implements the core ELIZA pattern-matching logic using
// C++ <regex>.  Contains 50+ regular expressions organized
// by the seven required category groups plus extras.
//
// Matching strategy:
//   - Patterns are tested via regex_search() in priority order
//     (most specific first, catch-all last).
//   - Each pattern group maps to a category string that the
//     response generator uses to select an appropriate reply.
// ============================================================

#include "pattern_matcher.h"
#include <regex>
#include <vector>
#include <string>

// --------------------------------------------------------
// PatternEntry: pairs a compiled regex with a category label.
// --------------------------------------------------------
struct PatternEntry {
    std::regex  pattern;
    std::string category;
};

// --------------------------------------------------------
// buildPatterns:
//   Constructs and returns the master list of pattern entries
//   in priority order.  icase makes every pattern
//   case-insensitive (input is already lowercased, but this
//   adds a safety net).
// --------------------------------------------------------
static std::vector<PatternEntry> buildPatterns() {
    // Alias for convenience
    auto flags = std::regex_constants::icase | std::regex_constants::ECMAScript;

    std::vector<PatternEntry> patterns = {

        // -------------------------------------------------------
        // GREETING patterns (8 patterns)
        // -------------------------------------------------------
        { std::regex(R"(\b(hello|hi|hey|howdy|greetings|good morning|good afternoon|good evening)\b)", flags), CAT_GREETING },
        { std::regex(R"(\bhow are you\b)",                               flags), CAT_GREETING },
        { std::regex(R"(\bnice to meet you\b)",                          flags), CAT_GREETING },
        { std::regex(R"(\bwhat('s| is) your name\b)",                    flags), CAT_GREETING },
        { std::regex(R"(\bwho are you\b)",                               flags), CAT_GREETING },
        { std::regex(R"(\bwhat can you do\b)",                           flags), CAT_GREETING },
        { std::regex(R"(\b(good|fine|okay|alright),? (thanks|thank you)\b)", flags), CAT_GREETING },
        { std::regex(R"(\bpleasure to (meet|talk|speak)\b)",             flags), CAT_GREETING },

        // -------------------------------------------------------
        // CONCERN / CRISIS patterns (8 patterns)
        // -------------------------------------------------------
        { std::regex(R"(\b(suicid|kill (myself|yourself|me)|end my life|want to die|don'?t want to live)\b)", flags), CAT_CONCERN },
        { std::regex(R"(\b(hate (myself|my life|everything|everyone))\b)", flags), CAT_CONCERN },
        { std::regex(R"(\b(depress(ed|ion)|hopeless|worthless|useless)\b)", flags), CAT_CONCERN },
        { std::regex(R"(\b(anxious|anxiety|panic(k(ed|ing))?|overwhelm)\b)", flags), CAT_CONCERN },
        { std::regex(R"(\b(abuse(d|r)?|trauma|violent|hurt me|harmed)\b)", flags), CAT_CONCERN },
        { std::regex(R"(\b(scared|terrified|afraid|fearful|frightened)\b)", flags), CAT_CONCERN },
        { std::regex(R"(\b(angry|furious|rage|livid|cannot stand|can'?t stand)\b)", flags), CAT_CONCERN },
        { std::regex(R"(\b(lonely|alone|isolated|no one cares)\b)",       flags), CAT_CONCERN },

        // -------------------------------------------------------
        // LOVE / RELATIONSHIP patterns (8 patterns)
        // -------------------------------------------------------
        { std::regex(R"(\b(love|in love|fall(ing)? in love)\b)",          flags), CAT_LOVE },
        { std::regex(R"(\b(girlfriend|boyfriend|partner|spouse|fiance|fiancee|husband|wife)\b)", flags), CAT_LOVE },
        { std::regex(R"(\b(relationship|dating|date|break ?up|broke up|divorce|separation)\b)", flags), CAT_LOVE },
        { std::regex(R"(\b(marriage|married|wedding|engagement|engaged)\b)", flags), CAT_LOVE },
        { std::regex(R"(\b(crush|infatuation|attracted to|feelings for)\b)", flags), CAT_LOVE },
        { std::regex(R"(\b(heartbreak|broken heart|miss (him|her|them|my ex))\b)", flags), CAT_LOVE },
        { std::regex(R"(\b(jealous|jealousy|cheating|cheated|unfaithful)\b)", flags), CAT_LOVE },
        { std::regex(R"(\b(trust issues|commitment|intimacy|connection)\b)", flags), CAT_LOVE },

        // -------------------------------------------------------
        // FINANCIAL patterns (6 patterns)
        // -------------------------------------------------------
        { std::regex(R"(\b(money|cash|funds|finances|financial)\b)",       flags), CAT_FINANCIAL },
        { std::regex(R"(\b(debt|loan|mortgage|credit|bankruptcy|bankrupt)\b)", flags), CAT_FINANCIAL },
        { std::regex(R"(\b(savings?|saving up|invest(ment|ing)?|stock(s)?|portfolio)\b)", flags), CAT_FINANCIAL },
        { std::regex(R"(\b(salary|wage|income|pay(check|rise|cut)?|afford)\b)", flags), CAT_FINANCIAL },
        { std::regex(R"(\b(budget|expenses?|spending|bills?|rent|mortgage)\b)", flags), CAT_FINANCIAL },
        { std::regex(R"(\b(rich|poor|wealth|broke|struggling financially)\b)", flags), CAT_FINANCIAL },

        // -------------------------------------------------------
        // WELLBEING patterns (7 patterns)
        // -------------------------------------------------------
        { std::regex(R"(\b(exercise|workout|gym|running|jogging|yoga|pilates|fitness)\b)", flags), CAT_WELLBEING },
        { std::regex(R"(\b(vitamin(s)?|supplement(s)?|nutrition|diet|eating (habit|healthy|well))\b)", flags), CAT_WELLBEING },
        { std::regex(R"(\b(sleep(ing)?|insomnia|rest(ing)?|tired|exhausted|fatigue)\b)", flags), CAT_WELLBEING },
        { std::regex(R"(\b(relax(ing|ation)?|meditat(e|ing|ion)|mindfulness|calm)\b)", flags), CAT_WELLBEING },
        { std::regex(R"(\b(stress(ed)?|burnout|overwhelm(ed)?|pressure)\b)", flags), CAT_WELLBEING },
        { std::regex(R"(\b(sick|ill(ness)?|disease|pain(ful)?|headache|migraine|chronic)\b)", flags), CAT_WELLBEING },
        { std::regex(R"(\b(mental health|therapy|therapist|counseling|psychiatrist|medication|meds)\b)", flags), CAT_WELLBEING },

        // -------------------------------------------------------
        // EDUCATION patterns (6 patterns)
        // -------------------------------------------------------
        { std::regex(R"(\b(school|university|college|campus|institution)\b)", flags), CAT_EDUCATION },
        { std::regex(R"(\b(course(s)?|class(es)?|lecture(s)?|seminar|curriculum)\b)", flags), CAT_EDUCATION },
        { std::regex(R"(\b(study(ing)?|homework|assignment|project|thesis|dissertation)\b)", flags), CAT_EDUCATION },
        { std::regex(R"(\b(student|professor|teacher|instructor|tutor|graduate|undergraduate)\b)", flags), CAT_EDUCATION },
        { std::regex(R"(\b(degree|diploma|certificate|major|minor|gpa|grade(s)?|exam(s)?|test(s)?)\b)", flags), CAT_EDUCATION },
        { std::regex(R"(\b(learn(ing)?|education|knowledge|skill(s)?|scholarship|tuition)\b)", flags), CAT_EDUCATION },

        // -------------------------------------------------------
        // ENTERTAINMENT patterns (6 patterns)
        // -------------------------------------------------------
        { std::regex(R"(\b(movie(s)?|film(s)?|cinema|theater|theatre|watch(ing)?)\b)",   flags), CAT_ENTERTAINMENT },
        { std::regex(R"(\b(music|concert|band|singer|album|playlist|song|listen(ing)?)\b)", flags), CAT_ENTERTAINMENT },
        { std::regex(R"(\b(restaurant|bar|pub|cafe|dine|dining|eat(ing)? out|food)\b)",  flags), CAT_ENTERTAINMENT },
        { std::regex(R"(\b(danc(e|ing)|party|parties|nightclub|club)\b)",                flags), CAT_ENTERTAINMENT },
        { std::regex(R"(\b(game(s)?|gaming|video game|sport(s)?|hobby|hobbies)\b)",     flags), CAT_ENTERTAINMENT },
        { std::regex(R"(\b(travel(ing)?|trip|vacation|holiday|adventure|explore)\b)",   flags), CAT_ENTERTAINMENT },

        // -------------------------------------------------------
        // FAMILY patterns (4 patterns)
        // -------------------------------------------------------
        { std::regex(R"(\b(mother|father|mom|dad|parent(s)?|sibling|brother|sister)\b)", flags), CAT_FAMILY },
        { std::regex(R"(\b(child(ren)?|son|daughter|baby|kid(s)?|family)\b)",            flags), CAT_FAMILY },
        { std::regex(R"(\b(aunt|uncle|cousin|grandparent|grandmother|grandfather|grandma|grandpa)\b)", flags), CAT_FAMILY },
        { std::regex(R"(\b(home|household|growing up|childhood|upbringing)\b)",           flags), CAT_FAMILY },

        // -------------------------------------------------------
        // WORK patterns (4 patterns)
        // -------------------------------------------------------
        { std::regex(R"(\b(job|work|career|profession|occupation|employ(ment|ed|er)?)\b)", flags), CAT_WORK },
        { std::regex(R"(\b(boss|manager|colleague|coworker|team|office|meeting(s)?)\b)",  flags), CAT_WORK },
        { std::regex(R"(\b(fired|laid off|resign(ed)?|quit|promotion|raise|demotion)\b)", flags), CAT_WORK },
        { std::regex(R"(\b(interview|application|resume|hire|hiring|workplace)\b)",        flags), CAT_WORK },

        // -------------------------------------------------------
        // GENERIC VERB patterns (6 patterns)
        //   These are the broad action verbs the assignment specifies.
        // -------------------------------------------------------
        { std::regex(R"(\b(have|has|having|possess(es|ed)?)\b)",            flags), CAT_GENERIC_VERB },
        { std::regex(R"(\b(purchas(e|ed|ing)?|buy|bought|shop(ping)?)\b)",  flags), CAT_GENERIC_VERB },
        { std::regex(R"(\b(know(s)?|knew|understand(s)?|understood|realize(d)?)\b)", flags), CAT_GENERIC_VERB },
        { std::regex(R"(\b(want(s|ed)?|wish(es|ed)?|hope(s|d)?|desire(d|s)?)\b)", flags), CAT_GENERIC_VERB },
        { std::regex(R"(\b(need(s|ed)?|require(s|d)?|must|have to|has to|got to)\b)", flags), CAT_GENERIC_VERB },
        { std::regex(R"(\b(think(s)?|thought|believe(d|s)?|feel(s)?|felt|seem(s|ed)?)\b)", flags), CAT_GENERIC_VERB },

        // -------------------------------------------------------
        // NEGATION patterns (2 patterns)
        // -------------------------------------------------------
        { std::regex(R"(\b(no|nope|never|not|nothing|nobody|nowhere|no one|cannot|can'?t|won'?t|don'?t|doesn'?t|didn'?t)\b)", flags), CAT_NEGATION },
        { std::regex(R"(\b(i do not know|i don'?t know|not sure|unsure|i'?m not sure)\b)", flags), CAT_NEGATION },

        // -------------------------------------------------------
        // AFFIRMATION patterns (2 patterns)
        // -------------------------------------------------------
        { std::regex(R"(\b(yes|yeah|yep|yup|sure|absolutely|definitely|of course|certainly)\b)", flags), CAT_AFFIRMATION },
        { std::regex(R"(\b(maybe|perhaps|possibly|i think so|i guess|could be)\b)", flags), CAT_AFFIRMATION },

        // -------------------------------------------------------
        // QUESTION patterns (2 patterns)
        // -------------------------------------------------------
        { std::regex(R"(^\s*(what|why|how|when|where|who|which|can you|could you|would you|do you|are you|is there)\b)", flags), CAT_QUESTION },
        { std::regex(R"(\?\s*$)",                                            flags), CAT_QUESTION },

        // -------------------------------------------------------
        // CATCH-ALL (always matches as the final fallback)
        // -------------------------------------------------------
        { std::regex(R"(.+)",                                                flags), CAT_CATCHALL }
    };

    return patterns;
}

// --------------------------------------------------------
// matchPattern:
//   Iterates through the prioritized pattern list and returns
//   the category string of the first matching regex.
//   Because the catch-all pattern (.+) always matches any
//   non-empty string, this function always returns a category.
// --------------------------------------------------------
std::string matchPattern(const std::string& input) {
    // Build pattern list once and reuse across calls
    static const std::vector<PatternEntry> patterns = buildPatterns();

    for (const auto& entry : patterns) {
        if (std::regex_search(input, entry.pattern)) {
            return entry.category;
        }
    }

    // Should never reach here because of the catch-all
    return CAT_CATCHALL;
}
