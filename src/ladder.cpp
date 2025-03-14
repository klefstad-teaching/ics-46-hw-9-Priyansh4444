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

bool is_adjacent(const string &word1, const string &word2)
{
    if (word1 == word2)
    {
        return true;
    }
    // Words differ by more than one character in length
    if (abs((int)word1.length() - (int)word2.length()) > 1)
    {
        return false;
    }

    // Same length - check for one character difference
    if (word1.length() == word2.length())
    {
        int diffs = 0;
        for (size_t i = 0; i < word1.length(); i++)
        {
            if (word1[i] != word2[i])
            {
                diffs++;
            }
            if (diffs > 1)
            {
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

    while (i < shorter.length() && j < longer.length())
    {
        if (shorter[i] != longer[j])
        {
            if (found_diff)
            {
                return false; // More than one difference
            }
            found_diff = true;
            j++; // Skip the character in longer word
        }
        else
        {
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

vector<string> generate_word_ladder(const string &begin_word, const string &end_word, const set<string> &word_list) {
    // Quick validations
    if (begin_word == end_word) return {};
    
    if (abs((int)begin_word.length() - (int)end_word.length()) > 1) {
        error(begin_word, end_word, "similar length");
        return {};
    }
    
    if (word_list.find(end_word) == word_list.end()) {
        error(begin_word, end_word, "valid dictionary");
        return {};
    }

    // BFS implementation
    queue<string> q;
    map<string, string> prev;  // Maps each word to its predecessor in the ladder
    
    q.push(begin_word);
    prev[begin_word] = "";  // Mark the starting point
    
    bool found = false;
    
    while (!q.empty() && !found) {
        string current = q.front();
        q.pop();
        
        // Check all possible neighbors by modifying one character at a time
        // For same length words - try changing each character
        if (current.length() == end_word.length()) {
            string temp = current;
            for (size_t i = 0; i < current.length(); i++) {
                char original = temp[i];
                for (char c = 'a'; c <= 'z'; c++) {
                    if (c == original) continue;
                    temp[i] = c;
                    
                    if (temp == end_word) {
                        prev[end_word] = current;
                        found = true;
                        break;
                    }
                    
                    if (word_list.find(temp) != word_list.end() && prev.find(temp) == prev.end()) {
                        q.push(temp);
                        prev[temp] = current;
                    }
                }
                temp[i] = original;
                if (found) break;
            }
        }
        
        // For words that differ by one character in length
        // Try inserting a character
        if (!found && current.length() + 1 == end_word.length()) {
            string temp = current;
            for (size_t i = 0; i <= current.length(); i++) {
                temp.insert(i, 1, ' ');  // Insert a placeholder
                for (char c = 'a'; c <= 'z'; c++) {
                    temp[i] = c;
                    
                    if (temp == end_word) {
                        prev[end_word] = current;
                        found = true;
                        break;
                    }
                    
                    if (word_list.find(temp) != word_list.end() && prev.find(temp) == prev.end()) {
                        q.push(temp);
                        prev[temp] = current;
                    }
                }
                temp.erase(i, 1);  // Remove the inserted character
                if (found) break;
            }
        }
        
        // Try deleting a character
        if (!found && current.length() == end_word.length() + 1) {
            string temp = current;
            for (size_t i = 0; i < current.length(); i++) {
                char deleted = temp[i];
                temp.erase(i, 1);
                
                if (temp == end_word) {
                    prev[end_word] = current;
                    found = true;
                    break;
                }
                
                if (word_list.find(temp) != word_list.end() && prev.find(temp) == prev.end()) {
                    q.push(temp);
                    prev[temp] = current;
                }
                
                temp.insert(i, 1, deleted);  // Restore the deleted character
                if (found) break;
            }
        }
    }
    
    if (!found) {
        error(begin_word, end_word, "connected");
        return {};
    }
    
    // Reconstruct the ladder
    vector<string> ladder;
    string current = end_word;
    while (current != "") {
        ladder.push_back(current);
        current = prev[current];
    }
    reverse(ladder.begin(), ladder.end());
    
    return ladder;
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