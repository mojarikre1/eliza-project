// ============================================================
// main.cpp
// Entry point for the ELIZA psychiatric chatbot simulation.
// Manages the main conversation loop, reads user input,
// delegates pattern matching and response generation,
// and tracks conversation history.
// ============================================================

#include <iostream>
#include <string>
#include "input_handler.h"
#include "pattern_matcher.h"
#include "response_generator.h"
#include "history_tracker.h"

int main() {
    // Welcome banner
    std::cout << "=================================================\n";
    std::cout << "  ELIZA - Psychiatric Interview Simulation\n";
    std::cout << "  (Type 'bye' to end the session)\n";
    std::cout << "=================================================\n\n";
    std::cout << "ELIZA: Hello. I am ELIZA, your session psychiatrist.\n"
              << "       How are you feeling today?\n\n";

    HistoryTracker history;   // tracks previous user responses
    std::string userInput;

    // Main conversation loop
    while (true) {
        std::cout << "You:   ";
        std::getline(std::cin, userInput);

        // Normalize the input (lowercase, trim whitespace)
        std::string normalized = normalizeInput(userInput);

        // Check for exit condition
        if (normalized == "bye") {
            std::cout << "\nELIZA: Goodbye. I hope our session has been helpful.\n"
                      << "       Take care of yourself.\n";
            break;
        }

        // Handle empty input gracefully
        if (normalized.empty()) {
            std::cout << "\nELIZA: Please, say something. I am here to listen.\n\n";
            continue;
        }

        // Detect if the user is repeating themselves
        if (history.isRepeat(normalized)) {
            std::cout << "\nELIZA: " << history.getRepeatPrompt() << "\n\n";
            continue;
        }

        // Record the input for future repeat detection
        history.record(normalized);

        // Match the input against known patterns and build a response
        std::string category = matchPattern(normalized);
        std::string response = generateResponse(category, normalized);

        std::cout << "\nELIZA: " << response << "\n\n";
    }

    return 0;
}
