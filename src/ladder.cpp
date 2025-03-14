#include "ladder.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <cmath>

using namespace std;
void error(string word1, string word2, string msg)
{
    cerr << "Error: " << word1 << " and " << word2 << " are not " << msg << " words" << endl;
}

bool edit_distance_within(const std::string &str1, const std::string &str2, int d)
{
    // Check if length difference exceeds the allowed edit distance
    if (abs((int)str1.length() - (int)str2.length()) > d)
        return false;

    // If strings are of equal length, count character differences
    if (str1.length() == str2.length())
    {
        int count = 0;
        for (size_t i = 0; i < str1.length(); ++i)
        {
            if (str1[i] != str2[i])
                count++;
            if (count > d)
                return false;
        }
        return true;
    }

    // Handle insertion/deletion between strings of different lengths
    const string &shorter = (str1.length() < str2.length()) ? str1 : str2;
    const string &longer = (str1.length() > str2.length()) ? str1 : str2;

    // Check if we can transform shorter to longer with at most d operations
    if (longer.length() - shorter.length() > d)
        return false;

    // Count required insertions/deletions and substitutions
    size_t i = 0, j = 0;
    int edits = 0;

    while (i < shorter.length() && j < longer.length())
    {
        if (shorter[i] != longer[j])
        {
            edits++;
            if (edits > d)
                return false;
            // Try to align by skipping character in longer string
            j++;
        }
        else
        {
            i++;
            j++;
        }
    }

    // Add remaining length difference to edit count
    edits += longer.length() - j;

    return edits <= d;
}

bool is_adjacent(const string &word1, const string &word2) {
    // Words differ by more than one character in length
    if (abs((int)word1.length() - (int)word2.length()) > 1) {
        return false;
    }
    
    // Same length - check for one character difference
    if (word1.length() == word2.length()) {
        int diffs = 0;
        for (size_t i = 0; i < word1.length(); i++) {
            if (word1[i] != word2[i]) {
                diffs++;
            }
            if (diffs > 1) {
                return false;
            }
        }
        return diffs == 1; // Must have exactly one difference
    }
    
    // Different length - check if one word can be derived from the other
    // by adding/removing one character
    const string &shorter = (word1.length() < word2.length()) ? word1 : word2;
    const string &longer = (word1.length() > word2.length()) ? word1 : word2;
    
    size_t i = 0, j = 0;
    bool found_diff = false;
    
    while (i < shorter.length() && j < longer.length()) {
        if (shorter[i] != longer[j]) {
            if (found_diff) {
                return false; // More than one difference
            }
            found_diff = true;
            j++; // Skip the character in longer word
        } else {
            i++;
            j++;
        }
    }
    
    // If we've gone through all characters in the shorter word
    // and we're at the last character of the longer word, they're adjacent
    return (i == shorter.length() && (j == longer.length() || j == longer.length() - 1)) ||
           (found_diff && i == shorter.length() && j == longer.length());
}

void load_words(set<string> &word_list, const string &file_name)
{
    ifstream file(file_name);
    string word;
    while (file >> word)
    {
        word_list.insert(word);
    }
}

vector<string> generate_word_ladder(const string &begin_word, const string &end_word, const set<string> &word_list)
{
    // If begin_word is already end_word, return trivial ladder
    if (begin_word == end_word)
    {
        return {begin_word};
    }

    // Check if words are of similar length
    if (begin_word.length() != end_word.length() &&
        abs((int)begin_word.length() - (int)end_word.length()) > 1)
    {
        error(begin_word, end_word, "similar length");
        return {};
    }

    // Check if end_word is in dictionary
    if (word_list.find(end_word) == word_list.end())
    {
        error(begin_word, end_word, "valid dictionary");
        return {};
    }

    queue<vector<string>> ladders;
    set<string> visited;

    ladders.push({begin_word});
    visited.insert(begin_word);

    while (!ladders.empty())
    {
        vector<string> current_ladder = ladders.front();
        ladders.pop();

        string current_word = current_ladder.back();

        // Check if we've reached the end word
        if (current_word == end_word)
        {
            return current_ladder;
        }

        // Try all words in the dictionary
        for (const string &word : word_list)
        {
            // Only process words we haven't visited yet
            if (visited.find(word) == visited.end() && is_adjacent(current_word, word))
            {
                visited.insert(word); // Mark as visited immediately
                vector<string> new_ladder = current_ladder;
                new_ladder.push_back(word);
                ladders.push(new_ladder);
            }
        }
    }

    // If we get here, no ladder was found
    error(begin_word, end_word, "connected");
    return {};
}

void print_word_ladder(const vector<string> &ladder)
{
    if (ladder.empty())
    {
        std::cout << "No word ladder found." << std::endl;
        return;
    }
    std::cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); ++i)
    {
        cout << ladder[i] << " ";
    }
    cout << endl;
}

void verify_word_ladder()
{
    return;
}