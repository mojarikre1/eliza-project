// ============================================================
// history_tracker.h
// Declares the HistoryTracker class used to detect repeated
// user responses and motivate the user to give new information.
// ============================================================

#ifndef HISTORY_TRACKER_H
#define HISTORY_TRACKER_H

#include <string>
#include <unordered_map>
#include <vector>

// HistoryTracker stores normalized user inputs and their
// occurrence counts.  When the same input appears more than
// once, the chatbot recognizes the repetition and responds
// accordingly instead of giving a standard reply.
class HistoryTracker {
public:
    HistoryTracker();

    // Records the normalized input string in the history log.
    void record(const std::string& input);

    // Returns true if the input has been seen before in this session.
    bool isRepeat(const std::string& input) const;

    // Returns one of several prompts encouraging the user to
    // say something new.  Cycles through a bank of prompts.
    std::string getRepeatPrompt();

private:
    // Maps normalized input -> number of times seen
    std::unordered_map<std::string, int> seen_;

    // Bank of prompts for repeated responses
    std::vector<std::string> repeatPrompts_;

    // Rotating index into repeatPrompts_
    size_t promptIndex_;
};

#endif // HISTORY_TRACKER_H
