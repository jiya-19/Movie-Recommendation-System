#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include "RecommenderSystem.h"

void displayMenu() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "   MOVIE RECOMMENDATION SYSTEM" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "  1. Get recommendations by movie" << std::endl;
    std::cout << "  2. Get recommendations by genres" << std::endl;
    std::cout << "  3. Exit" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Enter your choice (1-3): ";
}

int getUserChoice() {
    int choice;
    while (true) {
        if (std::cin >> choice) {
            if (choice >= 1 && choice <= 3) {
                return choice;
            } else {
                std::cout << "Please enter a number between 1 and 3: ";
            }
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number: ";
        }
    }
}


int getPositiveInteger(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            if (value > 0) {
                return value;
            } else {
                std::cout << "Please enter a positive number." << std::endl;
            }
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
        }
    }
}

std::string getValidGenre(const std::vector<std::string>& availableGenres) {
    std::string genre;
    
    while (true) {
        std::cout << "Enter genre: ";
        std::getline(std::cin, genre);
        
        // Trim whitespace
        size_t start = genre.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
            std::cout << "Genre cannot be empty. Try again." << std::endl;
            continue;
        }
        size_t end = genre.find_last_not_of(" \t\r\n");
        genre = genre.substr(start, end - start + 1);
        
        // Convert to lowercase for comparison
        std::string genreLower = genre;
        std::transform(genreLower.begin(), genreLower.end(), genreLower.begin(), ::tolower);
        
        // Check if genre exists
        bool found = false;
        for (const auto& g : availableGenres) {
            std::string gLower = g;
            std::transform(gLower.begin(), gLower.end(), gLower.begin(), ::tolower);
            if (gLower == genreLower) {
                genre = g; // Use the original case
                found = true;
                break;
            }
        }
        
        if (found) {
            return genre;
        } else {
            std::cout << "Genre not found. Available genres:" << std::endl;
            for (size_t i = 0; i < availableGenres.size(); i++) {
                std::cout << "  - " << availableGenres[i] << std::endl;
            }
        }
    }
}


void displayRecommendations(
    const std::vector<std::pair<Movie, double>>& recommendations,
    const std::string& context) {
    
    if (recommendations.empty()) {
        std::cout << "No recommendations found." << std::endl;
        return;
    }
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "     TOP " << recommendations.size() << " RECOMMENDATIONS" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Based on: " << context << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    for (size_t i = 0; i < recommendations.size(); i++) {
        const auto& rec = recommendations[i];
        const Movie& movie = rec.first;
        double similarity = rec.second;
        
        std::cout << (i + 1) << ". " << movie.getTitle() 
                  << " (" << movie.getYear() << ")" << std::endl;
        std::cout << "   Genres: ";
        
        std::vector<std::string> genres = movie.getGenres();
        for (size_t j = 0; j < genres.size(); j++) {
            std::cout << genres[j];
            if (j < genres.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
        
        std::cout << "   Similarity Score: " << (similarity * 100) << "%" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
    
    std::cout << "========================================\n" << std::endl;
}


void handleRecommendationsByMovie(RecommenderSystem& system) {
    std::string movieTitle;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "\nEnter the movie title: ";
    std::getline(std::cin, movieTitle);
    
    // Find the movie first to show info
    Movie* movie = system.findMovieByTitle(movieTitle);
    
    if (movie == nullptr) {
        std::cerr << "\n[ERROR] Movie not found: " << movieTitle << std::endl;
        std::cout << "Tip: Check the spelling or use option 1 to see all movies." << std::endl;
        return;
    }
    
    std::cout << "\nSelected: " << movie->toString() << std::endl;
    
    int topN = getPositiveInteger("Enter number of recommendations (e.g., 5): ");
    
    auto recommendations = system.getRecommendationsByMovie(movieTitle, topN);
    displayRecommendations(recommendations, "Similar to \"" + movie->getTitle() + "\"");
}


void handleRecommendationsByGenres(RecommenderSystem& system) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "  RECOMMENDATIONS BY GENRE" << std::endl;
    std::cout << "========================================" << std::endl;
    
    
    std::vector<std::string> preferredGenres;
    std::string genre;
    
    std::cout << "Enter genres one at a time (enter 'done' when finished):" << std::endl;
    
    while (true) {
        std::cout << "Add genre (or 'done' to finish): ";
        std::getline(std::cin, genre);
        
        // Trim whitespace
        size_t start = genre.find_first_not_of(" \t\r\n");
        if (start != std::string::npos) {
            size_t end = genre.find_last_not_of(" \t\r\n");
            genre = genre.substr(start, end - start + 1);
        }
        
        // Convert to lowercase for comparison
        std::string genreLower = genre;
        std::transform(genreLower.begin(), genreLower.end(), genreLower.begin(), ::tolower);
        
        if (genreLower == "done" || genreLower == "finished") {
            break;
        }
        
        if (genre.empty()) {
            continue;
        }
        
        // Check if genre exists
        std::vector<std::string> allGenres = system.getAllGenreList();
        bool found = false;
        for (const auto& g : allGenres) {
            std::string gLower = g;
            std::transform(gLower.begin(), gLower.end(), gLower.begin(), ::tolower);
            if (gLower == genreLower) {
                // Check if already added
                bool alreadyAdded = false;
                for (const auto& pg : preferredGenres) {
                    std::string pgLower = pg;
                    std::transform(pgLower.begin(), pgLower.end(), pgLower.begin(), ::tolower);
                    if (pgLower == genreLower) {
                        alreadyAdded = true;
                        break;
                    }
                }
                
                if (!alreadyAdded) {
                    preferredGenres.push_back(g);
                    std::cout << "  Added: " << g << std::endl;
                } else {
                    std::cout << "  Already added!" << std::endl;
                }
                found = true;
                break;
            }
        }
        
        if (!found && !genre.empty()) {
            std::cout << "  Genre not found. Try from the list above." << std::endl;
        }
    }
    
    if (preferredGenres.empty()) {
        std::cout << "No genres selected. Returning to menu." << std::endl;
        return;
    }
    
    std::cout << "\nSelected genres: ";
    for (size_t i = 0; i < preferredGenres.size(); i++) {
        std::cout << preferredGenres[i];
        if (i < preferredGenres.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    
    int topN = getPositiveInteger("Enter number of recommendations (e.g., 5): ");
    
    auto recommendations = system.getRecommendationsByGenres(preferredGenres, topN);
    
    std::string context = "Your preferred genres";
    displayRecommendations(recommendations, context);
}

int main() {
    std::cout << "\n";
    std::cout << "========================================" << std::endl;
    std::cout << "   MOVIE RECOMMENDATION SYSTEM" << std::endl;
    std::cout << "   Content-Based Filtering" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Create recommender system
    RecommenderSystem recommender;
    
    // Load movies from CSV
    std::string csvPath = "data/movies.csv";
    std::cout << "\n[INFO] Loading movies from " << csvPath << "..." << std::endl;
    
    if (!recommender.loadMoviesFromCSV(csvPath)) {
        std::cerr << "\n[ERROR] Failed to load movies. Please check if the CSV file exists." << std::endl;
        std::cerr << "[INFO] Expected path: " << csvPath << std::endl;
        return 1;
    }
    
    // Initialize the system 
    recommender.initializeSystem();
    
    std::cout << "\n[INFO] System ready! " << recommender.getMovieCount() 
              << " movies loaded." << std::endl;
    
    // Main loop
    while (true) {
        displayMenu();
        int choice = getUserChoice();
        
        switch (choice) {
            case 1:
                handleRecommendationsByMovie(recommender);
                break;
                
            case 2:
                handleRecommendationsByGenres(recommender);
                break;
                
            case 3:
                std::cout << "\n========================================" << std::endl;
                std::cout << "   Thank you for using the system!" << std::endl;
                std::cout << "   Goodbye!" << std::endl;
                std::cout << "========================================" << std::endl;
                return 0;
                
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    
    return 0;
}
