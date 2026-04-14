// ============================================================
// response_generator.cpp
// Implements response generation for each pattern category.
//
// Design:
//   - Each category has a vector of response templates.
//   - A static per-category index rotates through responses so
//     consecutive messages of the same type get varied replies.
//   - reflectPronouns() uses regex_replace() to mirror the user's
//     words back ("I am" -> "you are") creating an illusion of
//     understanding, the classic ELIZA technique.
// ============================================================

#include "response_generator.h"
#include "pattern_matcher.h"
#include <regex>
#include <vector>
#include <string>
#include <map>

// ============================================================
// reflectPronouns
//   Uses regex_replace() to transform first-person pronouns
//   to second-person so the echoed phrase feels empathetic.
//   Example: "I feel lost" -> "you feel lost"
// ============================================================
std::string reflectPronouns(const std::string& input) {
    auto flags = std::regex_constants::icase;
    std::string result = input;

    // Order matters: replace compound forms before single words
    // to avoid double-replacing (e.g. "my" inside "myself").
    result = std::regex_replace(result, std::regex(R"(\bi am\b)",    flags), "you are");
    result = std::regex_replace(result, std::regex(R"(\bi was\b)",   flags), "you were");
    result = std::regex_replace(result, std::regex(R"(\bi have\b)",  flags), "you have");
    result = std::regex_replace(result, std::regex(R"(\bi had\b)",   flags), "you had");
    result = std::regex_replace(result, std::regex(R"(\bi can\b)",   flags), "you can");
    result = std::regex_replace(result, std::regex(R"(\bi will\b)",  flags), "you will");
    result = std::regex_replace(result, std::regex(R"(\bi would\b)", flags), "you would");
    result = std::regex_replace(result, std::regex(R"(\bi could\b)", flags), "you could");
    result = std::regex_replace(result, std::regex(R"(\bmy\b)",      flags), "your");
    result = std::regex_replace(result, std::regex(R"(\bmine\b)",    flags), "yours");
    result = std::regex_replace(result, std::regex(R"(\bmyself\b)",  flags), "yourself");
    result = std::regex_replace(result, std::regex(R"(\bi\b)",       flags), "you");
    result = std::regex_replace(result, std::regex(R"(\bme\b)",      flags), "you");

    return result;
}

// ============================================================
// Response banks: one vector of strings per category.
// Each call to generateResponse() advances a per-category index
// so repeated messages in the same category cycle through replies.
// ============================================================

// Map from category -> current rotation index
static std::map<std::string, size_t> responseIndex;

// Helper: returns the next response from a bank, cycling via index
static std::string nextResponse(const std::string& category,
                                const std::vector<std::string>& bank) {
    size_t& idx = responseIndex[category];
    std::string response = bank[idx % bank.size()];
    idx++;
    return response;
}

// ============================================================
// generateResponse
//   Main dispatcher.  Selects the response bank matching the
//   category, then uses the pronoun-reflected input where the
//   template contains the placeholder "{echo}".
// ============================================================
std::string generateResponse(const std::string& category,
                             const std::string& input) {
    // Pronoun-reflected copy of the user input for echo responses
    std::string echo = reflectPronouns(input);

    // ----------------------------------------------------------
    // GREETING responses
    // ----------------------------------------------------------
    if (category == CAT_GREETING) {
        static const std::vector<std::string> bank = {
            "Hello. Please, tell me what's on your mind.",
            "Welcome. Make yourself comfortable. What brings you in today?",
            "I am glad you reached out. How can I help you?",
            "Good to meet you. What would you like to talk about?",
            "Hello. I am here to listen. What's troubling you?"
        };
        return nextResponse(category, bank);
    }

    // ----------------------------------------------------------
    // CONCERN / CRISIS responses
    // ----------------------------------------------------------
    if (category == CAT_CONCERN) {
        static const std::vector<std::string> bank = {
            "That sounds very serious. Can you tell me more about what you are feeling?",
            "I hear you. Those feelings are valid. What happened to bring this on?",
            "I am concerned about what you just shared. Have you spoken to someone about this?",
            "It takes courage to say that out loud. How long have you felt this way?",
            "Your safety is the most important thing. Is there someone you trust who knows how you feel?",
            "I want to understand. When these feelings arise, what do you usually do?",
            "That must be incredibly painful. Tell me — has something specific triggered this?",
            "I appreciate you telling me. Let's talk through this carefully. What does a typical day look like for you?"
        };
        return nextResponse(category, bank);
    }

    // ----------------------------------------------------------
    // LOVE / RELATIONSHIP responses
    // ----------------------------------------------------------
    if (category == CAT_LOVE) {
        static const std::vector<std::string> bank = {
            "Relationships can be complex. Tell me more about " + echo + ".",
            "How does that relationship make you feel?",
            "Love affects us deeply. What do you think you need from this relationship?",
            "Have these feelings changed how you see yourself?",
            "When did you first start feeling this way about them?",
            "How does this person respond to your feelings?",
            "What does a healthy relationship look like to you?",
            "It sounds like this person matters a great deal. What draws you to them?"
        };
        return nextResponse(category, bank);
    }

    // ----------------------------------------------------------
    // FINANCIAL responses
    // ----------------------------------------------------------
    if (category == CAT_FINANCIAL) {
        static const std::vector<std::string> bank = {
            "Financial worries can be very stressful. How is this affecting your daily life?",
            "Money concerns touch every part of our lives. What aspect worries you most?",
            "How long have you been dealing with this financial situation?",
            "Do you have people around you who can offer support with this?",
            "What would financial security look like for you?",
            "Has this financial stress affected your relationships or sleep?",
            "What small step might you take toward improving this situation?"
        };
        return nextResponse(category, bank);
    }

    // ----------------------------------------------------------
    // WELLBEING responses
    // ----------------------------------------------------------
    if (category == CAT_WELLBEING) {
        static const std::vector<std::string> bank = {
            "Physical wellbeing and mental health are closely connected. How has your body been feeling?",
            "How is your sleep? Poor rest can affect everything else.",
            "Are you making time for activities that recharge you?",
            "It sounds like you are working on your health. What motivated you to start?",
            "Stress can manifest physically. Where do you feel it most in your body?",
            "What does your self-care routine look like right now?",
            "Have you noticed any connection between your physical health and your mood?"
        };
        return nextResponse(category, bank);
    }

    // ----------------------------------------------------------
    // EDUCATION responses
    // ----------------------------------------------------------
    if (category == CAT_EDUCATION) {
        static const std::vector<std::string> bank = {
            "Education can be both exciting and overwhelming. How is that going for you?",
            "What do you enjoy most about your studies?",
            "Academic pressure can be significant. How are you managing it?",
            "What drew you to your field of study?",
            "Do you feel supported by your teachers or peers?",
            "How do you balance your academic life with personal time?",
            "What are your goals once you finish your education?"
        };
        return nextResponse(category, bank);
    }

    // ----------------------------------------------------------
    // ENTERTAINMENT responses
    // ----------------------------------------------------------
    if (category == CAT_ENTERTAINMENT) {
        static const std::vector<std::string> bank = {
            "Leisure time is important for wellbeing. Tell me more about that.",
            "How does that activity make you feel?",
            "Do you usually enjoy that on your own or with others?",
            "Hobbies and interests reveal a lot about us. What does this mean to you?",
            "Is there something in particular you are looking forward to?",
            "How much time do you set aside for enjoyment each week?",
            "Does this activity help you unwind from stress?"
        };
        return nextResponse(category, bank);
    }

    // ----------------------------------------------------------
    // FAMILY responses
    // ----------------------------------------------------------
    if (category == CAT_FAMILY) {
        static const std::vector<std::string> bank = {
            "Family relationships shape us profoundly. Can you say more?",
            "How would you describe your relationship with your family?",
            "Has this family dynamic changed recently?",
            "What role do you play within your family?",
            "Did your upbringing influence how you feel about that now?",
            "Do you feel understood by your family members?",
            "How does your family situation affect your overall happiness?"
        };
        return nextResponse(category, bank);
    }

    // ----------------------------------------------------------
    // WORK responses
    // ----------------------------------------------------------
    if (category == CAT_WORK) {
        static const std::vector<std::string> bank = {
            "Work takes up a large part of our lives. How are you feeling about it?",
            "Is your work fulfilling, or does it feel like a burden?",
            "How do you get along with the people at work?",
            "Has anything changed at work recently that is affecting you?",
            "Do you feel recognized and valued in your role?",
            "What would your ideal work situation look like?",
            "How much does your job influence your sense of identity?"
        };
        return nextResponse(category, bank);
    }

    // ----------------------------------------------------------
    // GENERIC VERB responses
    // ----------------------------------------------------------
    if (category == CAT_GENERIC_VERB) {
        static const std::vector<std::string> bank = {
            "Tell me more about that.",
            "And how does that make you feel?",
            "Why do you think " + echo + "?",
            "What led you to that point?",
            "How long have you felt this way?",
            "Is that something you have always experienced?",
            "What would change if that were different?"
        };
        return nextResponse(category, bank);
    }

    // ----------------------------------------------------------
    // NEGATION responses
    // ----------------------------------------------------------
    if (category == CAT_NEGATION) {
        static const std::vector<std::string> bank = {
            "Can you say a little more about that?",
            "What makes you feel that way?",
            "Is there something specific behind that feeling?",
            "Have you always felt this strongly about it?",
            "What would it look like if that were not the case?",
            "I see. What would you prefer instead?"
        };
        return nextResponse(category, bank);
    }

    // ----------------------------------------------------------
    // AFFIRMATION responses
    // ----------------------------------------------------------
    if (category == CAT_AFFIRMATION) {
        static const std::vector<std::string> bank = {
            "I see. Could you elaborate on that?",
            "That's interesting. What comes to mind when you think about it further?",
            "Good. And how does acknowledging that make you feel?",
            "Can you give me an example?",
            "What does that mean to you specifically?",
            "How certain are you about that?"
        };
        return nextResponse(category, bank);
    }

    // ----------------------------------------------------------
    // QUESTION responses
    // ----------------------------------------------------------
    if (category == CAT_QUESTION) {
        static const std::vector<std::string> bank = {
            "That's a thoughtful question. What do you think the answer is?",
            "Why does that question matter to you right now?",
            "What would it mean to you if you found the answer?",
            "I am more interested in what you believe. What's your feeling on it?",
            "Sometimes the question itself tells us something. What prompted you to ask?",
            "Let's explore that together. What do you already know about it?"
        };
        return nextResponse(category, bank);
    }

    // ----------------------------------------------------------
    // CATCH-ALL (fallback) responses
    // ----------------------------------------------------------
    {
        static const std::vector<std::string> bank = {
            "Please, go on.",
            "I see. Tell me more.",
            "Can you expand on that?",
            "How does that make you feel?",
            "That is quite interesting. Why do you think that is?",
            "What does that mean to you?",
            "I am listening. What else is on your mind?",
            "Can you say a bit more about " + echo + "?"
        };
        return nextResponse(CAT_CATCHALL, bank);
    }
}
