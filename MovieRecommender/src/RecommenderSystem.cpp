#include "RecommenderSystem.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>

// Constructor
RecommenderSystem::RecommenderSystem() : genreCount(0) {
}

// Destructor
RecommenderSystem::~RecommenderSystem() {
    // Vectors clean up themselves
}

std::string RecommenderSystem::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

std::vector<std::string> RecommenderSystem::parseGenres(const std::string& genreString) {
    std::vector<std::string> genres;
    std::stringstream ss(genreString);
    std::string genre;
    
    while (std::getline(ss, genre, ',')) {
        std::string trimmedGenre = trim(genre);
        if (!trimmedGenre.empty()) {
            genres.push_back(trimmedGenre);
            uniqueGenres.insert(trimmedGenre);  // Add to set of unique genres
        }
    }
    
    return genres;
}

void RecommenderSystem::extractUniqueGenres() {
    uniqueGenres.clear();
    
    // Iterate through all movies and collect genres
    for (const auto& movie : movies) {
        std::vector<std::string> movieGenres = movie.getGenres();
        for (const auto& genre : movieGenres) {
            uniqueGenres.insert(genre);
        }
    }
    
    genreCount = uniqueGenres.size();
    std::cout << "[INFO] Found " << genreCount << " unique genres in the dataset." << std::endl;
}

void RecommenderSystem::buildGenreIndexMap() {
    genreIndexMap.clear();
    
    int index = 0;
    for (const auto& genre : uniqueGenres) {
        genreIndexMap[genre] = index;
        index++;
    }
    
    std::cout << "[INFO] Genre index map built successfully." << std::endl;
}

void RecommenderSystem::createFeatureVectors() {
    for (auto& movie : movies) {
        std::vector<double> featureVector(genreCount, 0.0);
        
        std::vector<std::string> movieGenres = movie.getGenres();
        for (const auto& genre : movieGenres) {
            if (genreIndexMap.find(genre) != genreIndexMap.end()) {
                int index = genreIndexMap[genre];
                featureVector[index] = 1.0;
            }
        }
        
        movie.setFeatureVector(featureVector);
    }
    
    std::cout << "[INFO] Feature vectors created for all movies." << std::endl;
}

double RecommenderSystem::computeCosineSimilarity(const std::vector<double>& vec1, 
                                                    const std::vector<double>& vec2) {
    // Handle edge cases
    if (vec1.empty() || vec2.empty() || vec1.size() != vec2.size()) {
        return 0.0;
    }
    
    double dotProduct = 0.0;    // A · B (numerator)
    double norm1 = 0.0;         // ||A|| 
    double norm2 = 0.0;         // ||B||
    
    // Calculate dot product and magnitudes
    for (size_t i = 0; i < vec1.size(); i++) {
        dotProduct += vec1[i] * vec2[i];
        norm1 += vec1[i] * vec1[i];
        norm2 += vec2[i] * vec2[i];
    }
    
    // Calculate norms (magnitudes)
    norm1 = std::sqrt(norm1);
    norm2 = std::sqrt(norm2);
    
    // Handle division by zero (if any vector is all zeros)
    if (norm1 == 0.0 || norm2 == 0.0) {
        return 0.0;
    }
    
    // Calculate cosine similarity
    return dotProduct / (norm1 * norm2);
}

void RecommenderSystem::parseCSVLine(const std::string& line, 
                                      std::string& title, 
                                      std::string& yearStr, 
                                      std::string& genresStr) {
    size_t pos = 0;
    size_t fieldStart = 0;
    int fieldNum = 0;
    
    while (pos < line.length()) {
        // Check if we're at a quoted field
        if (line[pos] == '"') {
            // Find the closing quote
            size_t closeQuote = pos + 1;
            while (closeQuote < line.length()) {
                if (line[closeQuote] == '"') {
                    // Check if it's an escaped quote (two quotes in a row)
                    if (closeQuote + 1 < line.length() && line[closeQuote + 1] == '"') {
                        closeQuote += 2; // Skip escaped quote
                    } else {
                        break; // Found closing quote
                    }
                } else {
                    closeQuote++;
                }
            }
            
            // Extract the quoted field content
            std::string fieldContent = line.substr(pos + 1, closeQuote - pos - 1);
            
            // Handle escaped quotes (replace "" with ")
            size_t escapePos = 0;
            while ((escapePos = fieldContent.find("\"\"", escapePos)) != std::string::npos) {
                fieldContent.replace(escapePos, 2, "\"");
                escapePos++;
            }
            
            if (fieldNum == 0) {
                title = this->trim(fieldContent);
            } else if (fieldNum == 1) {
                yearStr = this->trim(fieldContent);
            } else if (fieldNum == 2) {
                genresStr = this->trim(fieldContent);
            }
            
            // Move past the closing quote
            pos = closeQuote + 1;
            
            // Skip the comma after the quoted field if present
            if (pos < line.length() && line[pos] == ',') {
                pos++;
            }
        } else if (line[pos] == ',') {
            // End of field
            std::string fieldContent = line.substr(fieldStart, pos - fieldStart);
            
            if (fieldNum == 0) {
                title = this->trim(fieldContent);
            } else if (fieldNum == 1) {
                yearStr = this->trim(fieldContent);
            } else if (fieldNum == 2) {
                genresStr = this->trim(fieldContent);
            }
            
            fieldNum++;
            fieldStart = pos + 1;
        }
        
        pos++;
    }
    
    // Handle the last field
    if (fieldStart < line.length()) {
        std::string fieldContent = line.substr(fieldStart);
        
        if (fieldNum == 0) {
            title = this->trim(fieldContent);
        } else if (fieldNum == 1) {
            yearStr = this->trim(fieldContent);
        } else if (fieldNum == 2) {
            genresStr = this->trim(fieldContent);
        }
    }
}

bool RecommenderSystem::loadMoviesFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open file: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    bool isFirstLine = true;
    int lineNumber = 0;
    
    // Read file line by line
    while (std::getline(file, line)) {
        lineNumber++;
        
        // Skip header line
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }
        
        // Skip empty lines
        if (line.empty()) continue;
        
        // Parse CSV line - handle quoted fields properly
        std::string title, yearStr, genresStr;
        this->parseCSVLine(line, title, yearStr, genresStr);
        
        // Skip if title is empty
        if (title.empty()) {
            continue;
        }
        
        // Skip if year is empty 
        if (yearStr.empty()) {
            continue;
        }
        
        // Convert year to integer, skip if invalid
        int year;
        try {
            year = std::stoi(yearStr);
        } catch (const std::exception& e) {
            continue;
        }
        
        // Parse genres
        std::vector<std::string> genres = parseGenres(genresStr);
        
        // Create and add movie to the collection
        Movie movie(title, year, genres);
        movies.push_back(movie);
    }
    
    file.close();
    std::cout << "[INFO] Successfully loaded " << movies.size() << " movies from " << filename << std::endl;
    return true;
}

void RecommenderSystem::initializeSystem() {
    std::cout << "[INFO] Initializing recommendation system..." << std::endl;
    extractUniqueGenres();
    buildGenreIndexMap();
    createFeatureVectors();
    std::cout << "[INFO] System initialized successfully!" << std::endl;
}

Movie* RecommenderSystem::findMovieByTitle(const std::string& title) {
    std::string searchTitle = trim(title);
    
    // Convert to lowercase for case-insensitive search
    std::transform(searchTitle.begin(), searchTitle.end(), searchTitle.begin(), ::tolower);
    
    for (auto& movie : movies) {
        std::string movieTitle = movie.getTitle();
        std::transform(movieTitle.begin(), movieTitle.end(), movieTitle.begin(), ::tolower);
        
        if (movieTitle == searchTitle) {
            return &movie;
        }
    }
    
    return nullptr;
}

std::vector<std::pair<Movie, double>> RecommenderSystem::getRecommendationsByMovie(
    const std::string& movieTitle, int topN) {
    
    std::vector<std::pair<Movie, double>> recommendations;
    
    // Find the source movie
    Movie* sourceMovie = findMovieByTitle(movieTitle);
    
    if (sourceMovie == nullptr) {
        std::cerr << "[ERROR] Movie not found: " << movieTitle << std::endl;
        return recommendations;
    }
    
    std::vector<double> sourceVector = sourceMovie->getFeatureVector();
    
    // Calculate similarity with all other movies
    for (auto& movie : movies) {
        // Skip the source movie itself
        if (movie.getTitle() == sourceMovie->getTitle()) {
            continue;
        }
        
        double similarity = computeCosineSimilarity(sourceVector, movie.getFeatureVector());
        recommendations.push_back(std::make_pair(movie, similarity));
    }
    
    // Sort by similarity score
    std::sort(recommendations.begin(), recommendations.end(), 
              [](const std::pair<Movie, double>& a, const std::pair<Movie, double>& b) {
                  return a.second > b.second;
              });
    
    // Return top N recommendations
    if (topN > static_cast<int>(recommendations.size())) {
        topN = recommendations.size();
    }
    
    std::vector<std::pair<Movie, double>> topRecommendations(
        recommendations.begin(), 
        recommendations.begin() + topN
    );
    
    return topRecommendations;
}

std::vector<std::pair<Movie, double>> RecommenderSystem::getRecommendationsByGenres(
    const std::vector<std::string>& preferredGenres, int topN) {
    
    std::vector<std::pair<Movie, double>> recommendations;
    
    // Create a feature vector for the preferred genres
    std::vector<double> preferredVector(genreCount, 0.0);
    
    for (const auto& genre : preferredGenres) {
        std::string trimmedGenre = trim(genre);
        if (genreIndexMap.find(trimmedGenre) != genreIndexMap.end()) {
            int index = genreIndexMap[trimmedGenre];
            preferredVector[index] = 1.0;
        }
    }
    
    // Calculate similarity with all movies
    for (auto& movie : movies) {
        double similarity = computeCosineSimilarity(preferredVector, movie.getFeatureVector());
        recommendations.push_back(std::make_pair(movie, similarity));
    }
    
    // Sort by similarity score (descending order)
    std::sort(recommendations.begin(), recommendations.end(), 
              [](const std::pair<Movie, double>& a, const std::pair<Movie, double>& b) {
                  return a.second > b.second;
              });
    
    // Return top N recommendations
    if (topN > static_cast<int>(recommendations.size())) {
        topN = recommendations.size();
    }
    
    std::vector<std::pair<Movie, double>> topRecommendations(
        recommendations.begin(), 
        recommendations.begin() + topN
    );
    
    return topRecommendations;
}

void RecommenderSystem::displayAllMovies() const {
    std::cout << "\n========================================" << std::endl;
    std::cout << "         ALL AVAILABLE MOVIES           " << std::endl;
    std::cout << "========================================" << std::endl;
    
    for (size_t i = 0; i < movies.size(); i++) {
        std::cout << (i + 1) << ". " << movies[i].toString() << std::endl;
    }
    
    std::cout << "========================================\n" << std::endl;
}

void RecommenderSystem::displayGenreList() const {
    std::cout << "\n========================================" << std::endl;
    std::cout << "         AVAILABLE GENRES               " << std::endl;
    std::cout << "========================================" << std::endl;
    
    int index = 1;
    for (const auto& genre : uniqueGenres) {
        std::cout << index << ". " << genre << std::endl;
        index++;
    }
    
    std::cout << "========================================\n" << std::endl;
}

int RecommenderSystem::getMovieCount() const {
    return movies.size();
}

std::vector<std::string> RecommenderSystem::getAllGenreList() const {
    return std::vector<std::string>(uniqueGenres.begin(), uniqueGenres.end());
}
