# Makefile for Movie Recommendation System

# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
TARGET = movie_recommender

# Source files
SRCS = src/main.cpp src/Movie.cpp src/RecommenderSystem.cpp
OBJS = $(SRCS:.cpp=.o)

# Default target - build the project
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files to object files
src/main.o: src/main.cpp include/RecommenderSystem.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/Movie.o: src/Movie.cpp include/Movie.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/RecommenderSystem.o: src/RecommenderSystem.cpp include/RecommenderSystem.h include/Movie.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program (from MovieRecommender directory)
run: $(TARGET)
	./$(TARGET)

# Build and run
build-run: clean all
	./$(TARGET)

.PHONY: all clean run build-run
