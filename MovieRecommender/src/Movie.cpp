#include "Movie.h"
#include <sstream>
#include <iostream>

// Default constructor
Movie::Movie() : year(0) {
}

// Parameterized constructor
Movie::Movie(const std::string& title, int year, const std::vector<std::string>& genres)
    : title(title), year(year), genres(genres) {
}

// Getters
std::string Movie::getTitle() const {
    return title;
}

int Movie::getYear() const {
    return year;
}

std::vector<std::string> Movie::getGenres() const {
    return genres;
}

std::vector<double> Movie::getFeatureVector() const {
    return featureVector;
}

// Setters
void Movie::setTitle(const std::string& title) {
    this->title = title;
}

void Movie::setYear(int year) {
    this->year = year;
}

void Movie::setGenres(const std::vector<std::string>& genres) {
    this->genres = genres;
}

void Movie::setFeatureVector(const std::vector<double>& featureVector) {
    this->featureVector = featureVector;
}

// Utility function to display movie information
std::string Movie::toString() const {
    std::ostringstream oss;
    oss << title << " (" << year << ") - Genres: ";
    for (size_t i = 0; i < genres.size(); ++i) {
        oss << genres[i];
        if (i < genres.size() - 1) {
            oss << ", ";
        }
    }
    return oss.str();
}

// Destructor
Movie::~Movie() {
    // Nothing to clean up - vectors handle their own memory
}
