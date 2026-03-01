#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include <vector>

class Movie {
private:
    std::string title;          // Movie title
    int year;                   // Release year
    std::vector<std::string> genres;  // List of genres
    std::vector<double> featureVector; // Feature vector for similarity calculation

public:
    // Constructors
    Movie();
    Movie(const std::string& title, int year, const std::vector<std::string>& genres);
    
    // Getters
    std::string getTitle() const;
    int getYear() const;
    std::vector<std::string> getGenres() const;
    std::vector<double> getFeatureVector() const;
    
    // Setters
    void setTitle(const std::string& title);
    void setYear(int year);
    void setGenres(const std::vector<std::string>& genres);
    void setFeatureVector(const std::vector<double>& featureVector);
    
    // Utility functions
    std::string toString() const;
    
    // Destructor
    ~Movie();
};

#endif // MOVIE_H
