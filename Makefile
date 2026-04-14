CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -o eliza

SRCS = main.cpp input_handler.cpp pattern_matcher.cpp \
       response_generator.cpp history_tracker.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRCS)

clean:
	rm -f eliza
