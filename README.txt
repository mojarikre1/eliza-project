===============================================================
  ELIZA Psychiatric Chatbot Simulation
  README.txt
===============================================================

GROUP MEMBERS
-------------
  [Mcdavis Ojarikre]

FILES
-----
  main.cpp               - Entry point and conversation loop
  input_handler.h/.cpp   - Input normalization (lowercase, trim)
  pattern_matcher.h/.cpp - 50+ regex pattern rules and categorization
  response_generator.h/.cpp - Response banks and pronoun reflection
  history_tracker.h/.cpp - Repeat-detection using unordered_map
  README.txt             - This file
  Makefile               - Optional build script

HOW TO COMPILE
--------------
Option A — Using the Makefile:

  make

Option B — Manual compilation with g++ on CSE machines:

  g++ -std=c++17 -o eliza \
      main.cpp \
      input_handler.cpp \
      pattern_matcher.cpp \
      response_generator.cpp \
      history_tracker.cpp

Note: C++11 or later is required for <regex>.
      C++17 is recommended.  Both are available on CSE Linux machines.

HOW TO RUN
----------
  ./eliza

The program will print a welcome banner and begin the conversation.
Type naturally; end the session by typing:

  bye

PROGRAM BEHAVIOR
----------------
- Every user input receives a psychiatrist-style response.
- Repeated identical inputs are detected and the user is prompted
  to elaborate further rather than receiving a generic reply.
- Input matching uses 50+ regular expressions across 14 categories:
    greeting, generic_verb, love, concern, financial, wellbeing,
    education, entertainment, family, work, negation, affirmation,
    question, catchall.
- Pronoun reflection (I -> you, my -> your) is applied using
  regex_replace() to give responses an empathetic tone.
===============================================================
