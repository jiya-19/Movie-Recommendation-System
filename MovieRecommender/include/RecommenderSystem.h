#ifndef RECOMMENDER_SYSTEM_H
#define RECOMMENDER_SYSTEM_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include "Movie.h"

class RecommenderSystem {
private:
    std::vector<Movie> movies;                    // List of all movies
    std::set<std::string> uniqueGenres;           // Set of all unique genres
    std::map<std::string, int> genreIndexMap;    // Genre name to index mapping
    int genreCount;                               // Total number of unique genres

    // Private member functions
    std::string trim(const std::string& str);
    std::vector<std::string> parseGenres(const std::string& genreString);
    void extractUniqueGenres();
    void buildGenreIndexMap();
    void createFeatureVectors();
    double computeCosineSimilarity(const std::vector<double>& vec1, 
                                    const std::vector<double>& vec2);
    void parseCSVLine(const std::string& line, std::string& title, 
                      std::string& yearStr, std::string& genresStr);

public:
    // Constructor and Destructor
    RecommenderSystem();
    ~RecommenderSystem();

    // Main functionality
    bool loadMoviesFromCSV(const std::string& filename);
    void initializeSystem();
    
    // Recommendation functions
    std::vector<std::pair<Movie, double>> getRecommendationsByMovie(
        const std::string& movieTitle, int topN);
    
    std::vector<std::pair<Movie, double>> getRecommendationsByGenres(
        const std::vector<std::string>& preferredGenres, int topN);
    
    // Utility functions
    void displayAllMovies() const;
    void displayGenreList() const;
    Movie* findMovieByTitle(const std::string& title);
    int getMovieCount() const;
    std::vector<std::string> getAllGenreList() const;
};

#endif // RECOMMENDER_SYSTEM_H
