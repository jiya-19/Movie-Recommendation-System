# 🎬 Movie Recommendation System

A content-based movie recommendation system built from scratch using C++ and cosine similarity.

## 📌 Project Overview

This project demonstrates how to build a movie recommendation system using **content-based filtering** - one of the fundamental approaches in recommender systems. The system recommends movies based on genre similarity, using mathematical techniques like **one-hot encoding** and **cosine similarity**.

## 🧠 What is Content-Based Filtering?

Content-based filtering recommends items based on their features (in this case, genres). Here's how it works:

1. **Feature Extraction**: Convert movie genres into numerical vectors (one-hot encoding)
2. **Similarity Calculation**: Compare movies using cosine similarity
3. **Ranking**: Sort by similarity score and return top N recommendations

### Simple Analogy
Think of movies as points in space. Movies with similar genres are "closer" together. The recommendation system finds the nearest movies to your selected movie!

## 📊 Cosine Similarity Explained Simply

Cosine similarity measures how similar two vectors are by calculating the cosine of the angle between them:

- **1.0** = Identical (perfect match)
- **0.0** = No similarity (completely different)
- **-1.0** = Opposite (rare in this case)

### Formula
```
Cosine Similarity = (A · B) / (||A|| × ||B||)
```

Where:
- A · B = Dot product of vectors
- ||A|| = Length of vector A
- ||B|| = Length of vector B

## 📁 Project Structure

```
MovieRecommender/
├── data/
│   └── movies.csv           # Movie dataset
├── include/
│   ├── Movie.h              # Movie class header
│   └── RecommenderSystem.h  # Recommender system header
├── src/
│   ├── main.cpp             # Main program & menu
│   ├── Movie.cpp            # Movie class implementation
│   └── RecommenderSystem.cpp # Core recommendation logic
├── Makefile                 # Build instructions
└── README.md                # This file
```

## 🚀 How to Build & Run

### Prerequisites
- C++ compiler (g++ or clang++)
- Make (optional, but recommended)

### Build Steps

```
bash
# Navigate to project directory
cd MovieRecommender

# Build the project
make

# Run the program
./movie_recommender
```

### Or compile manually:

```
bash
g++ -std=c++17 -Iinclude -o movie_recommender src/main.cpp src/Movie.cpp src/RecommenderSystem.cpp
./movie_recommender
```

## 💡 How to Use

The system provides two ways to get recommendations:

### Option 1: By Movie
Select a movie you like, and the system finds similar movies based on shared genres.

### Option 2: By Genre
Enter your preferred genres, and the system recommends movies matching those genres.

### Sample Dataset
The project includes 20 movies spanning genres like:
- Action, Adventure, Comedy, Crime, Drama
- Romance, Sci-Fi, Thriller, Animation, Fantasy

## 🎯 Features

- ✅ Dynamic genre extraction from CSV
- ✅ One-hot encoding for feature vectors
- ✅ Cosine similarity implementation (no external ML libraries)
- ✅ Interactive menu-driven interface
- ✅ Input validation and error handling
- ✅ Clean, modular OOP design
- ✅ Beginner-friendly with detailed comments

## 📈 Performance

### Time Complexity
- **Loading movies**: O(M) where M = number of movies
- **Building feature vectors**: O(M × G) where G = number of genres
- **Getting recommendations**: O(M × G) per query

### Space Complexity
- **Movies storage**: O(M × G)
- **Genre index map**: O(G)
- **Feature vectors**: O(M × G)

### Scalability
- Works efficiently with 100+ movies
- For 1000+ movies: Consider indexing or caching similarity scores

## 🔧 Future Improvements

Want to take this further? Here are internship-level enhancements:

1. **Weighted Genres**: Give different weights to different genres
2. **Rating Integration**: Consider movie ratings in recommendations
3. **Hybrid Scoring**: Combine content-based and collaborative filtering
4. **User Preferences**: Save and learn from user feedback
5. **Command-line Arguments**: Allow passing CSV path as argument
6. **Additional Features**: Include actors, directors, year as features


## 🤝 Contributing

Feel free to fork this project and add your own improvements!
