// Winter'24
// Instructor: Diba Mirza
// Student name: Zhe Wang
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
#include <map>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[0] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile(argv[1]);

    if (movieFile.fail())
    {
        cerr << "Could not open file " << argv[1];
        exit(1);
    }

    // Create an object of a STL data-structure to store all the movies
    map<string, double> movies;
    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating
    while (getline(movieFile, line) && parseLine(line, movieName, movieRating))
    {
        // Use std::string movieName and double movieRating
        // to construct your Movie objects
        // cout << movieName << " has rating " << movieRating << endl;
        // insert elements into your data structure
        movies[movieName] = movieRating; // insert into the sorted map
    }

    movieFile.close();

    if (argc == 2)
    {
        // print all the movies in ascending alphabetical order of movie names
        for (auto &[name, rating] : movies)
        {
            cout << name << ", " << fixed << setprecision(1) << rating << "\n";
        }
        return 0;
    }

    ifstream prefixFile(argv[2]);

    if (prefixFile.fail())
    {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline(prefixFile, line))
    {
        if (!line.empty())
        {
            prefixes.push_back(line);
        }
    }
    prefixFile.close();
    //  For each prefix,
    //  Find all movies that have that prefix and store them in an appropriate data structure
    //  If no movie with that prefix exists print the following message
    vector<Movie> bestResults(prefixes.size(), {"", -1.0});
    for (int i = 0; i < (int)prefixes.size(); i++)
    {
        const string &prefix = prefixes[i];

        auto it = movies.lower_bound(prefix);

        vector<Movie> matches;
        while (it != movies.end() &&
               it->first.substr(0, prefix.size()) == prefix)
        {
            matches.push_back({it->first, it->second});
            ++it;
        }

        if (matches.empty())
        {
            cout << "No movies found with prefix " << "<replace with prefix>" << endl;
        }
        else
        {

            sort(matches.begin(), matches.end(), [](const Movie &a, const Movie &b)
                 {
                if (a.rating != b.rating) return a.rating > b.rating;
                return a.name < b.name; });

            for (auto &m : matches)
            {
                cout << m.name << ", " << fixed << setprecision(1) << m.rating << "\n";
            }

            // After sorting, index 0 is the best movie.
            bestResults[i] = matches[0];
        }
    }

    // Print all "Best movie" lines at the end, in prefix order.
    for (int i = 0; i < (int)prefixes.size(); i++)
    {
        if (bestResults[i].rating >= 0)
        {
            cout << "Best movie with prefix " << "<replace with prefix>" << " is: " << "replace with movie name" << " with rating " << std::fixed << std::setprecision(1) << "replace with movie rating" << endl;
        }
    }

    return 0;
}

/* Add your run time analysis for part 3 of the assignment here as commented block*/
/* Part 3: Time and Space Complexity Analysis
 *
 * Parameters:
 *   n = number of movies in the dataset
 *   m = number of prefixes
 *   k = max number of movies matching a single prefix
 *   l = max number of characters in a movie name
 *
 *
 * Part 3a
 *
 * For each of the m prefixes, the code does:
 *   1. map::lower_bound(prefix)  :  O(l * log n)
 *      The map does log n node comparisons, each costing O(l)
 *      in the worst case for string comparison.
 *   2. Walk through k matching entries, checking substr each time : O(k * l)
 *   3. Sort k matches by rating and name : O(k log k)
 *
 * Per prefix: O(l * log n + k * l + k log k)
 * Total:      O(m * (l * log n + k * l + k log k))
 * Part 3b
 *
 * The map stores n entries, each name up to l characters:  O(n * l)
 * The temporary matches vector holds up to k movies:       O(k * l)
 * The bestResults vector holds m entries:                  O(m * l)
 *
 * The map dominates, so:
 * Total: O(n * l)
 * (with O(k * l + m * l) additional working space)
 * Part 3c: Tradeoffs
 *
 * I designed for low time complexity.
 * Target: O(m * (l * log n + k log k))
 *
 * The key decision was using std::map + lower_bound so that each prefix search starts at the right position in O(log n) rather than scanning all n movies linearly.
 * I was also able to keep space complexity low. Since I did not
 * pre-build any prefix structure, the space stays at O(n * l), just enough to hold the data.
 * A trie would give faster O(l) lookups but at much higher space cost. std::map gave a good balance of both goals.
 */
bool parseLine(string &line, string &movieName, double &movieRating)
{
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex + 1));
    if (movieName[0] == '\"')
    {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}