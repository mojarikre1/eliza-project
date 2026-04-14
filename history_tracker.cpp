// ============================================================
// history_tracker.cpp
// Implements the HistoryTracker class.
//
// Data structure choice:
//   std::unordered_map<std::string, int> provides O(1) average
//   lookup and insertion, which is ideal for checking whether
//   a normalized string has been seen before in the session.
// ============================================================

#include "history_tracker.h"

// Constructor: initialize the prompt bank and rotation index.
HistoryTracker::HistoryTracker()
    : promptIndex_(0)
{
    repeatPrompts_ = {
        "You have said that before. Can you think of another way to express what you are feeling?",
        "We have already covered that. Let's try to go deeper — what else is there?",
        "I notice you keep returning to that. Why do you think that is?",
        "You seem to come back to this thought often. What does it represent for you?",
        "Perhaps we should try a different angle. Is there something else connected to this?",
        "Repeating the same thought can be a sign of something important. What do you think it means?"
    };
}

// record:
//   Inserts or increments the count for the given input string.
void HistoryTracker::record(const std::string& input) {
    seen_[input]++;
}

// isRepeat:
//   Returns true if the input has been seen at least once before.
//   Note: we check count > 0 BEFORE calling record(), so the
//   first occurrence is never considered a repeat.
bool HistoryTracker::isRepeat(const std::string& input) const {
    auto it = seen_.find(input);
    return (it != seen_.end() && it->second > 0);
}

// getRepeatPrompt:
//   Returns the next prompt from the bank, cycling with wraparound.
std::string HistoryTracker::getRepeatPrompt() {
    std::string prompt = repeatPrompts_[promptIndex_ % repeatPrompts_.size()];
    promptIndex_++;
    return prompt;
}
