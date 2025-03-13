#include "ladder.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <unordered_set>
#include <map>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

void error(string word1, string word2, string msg)
{
    cerr << "Error: " << word1 << " and " << word2 << " are not " << msg << " words" << endl;
}

bool is_adjacent(const string &word1, const string &word2)
{
    // Early exit: check if string lengths differ by more than 1
    int length_diff = abs((int)word1.length() - (int)word2.length());
    if (length_diff > 1)
        return false;

    // Case 1: Same length - exactly one character different
    if (word1.length() == word2.length())
    {
        int diff = 0;
        for (size_t i = 0; i < word1.length(); ++i)
        {
            if (word1[i] != word2[i])
            {
                diff++;
                if (diff > 1)
                    return false;
            }
        }
        return diff == 1;
    }
    // Case 2: Length differs by 1 - insertion or deletion
    else // length_diff must be exactly 1 here
    {
        const string &shorter = (word1.length() < word2.length()) ? word1 : word2;
        const string &longer = (word1.length() > word2.length()) ? word1 : word2;

        size_t i = 0, j = 0;
        bool diff_found = false;

        while (i < shorter.length() && j < longer.length())
        {
            if (shorter[i] != longer[j])
            {
                if (diff_found)
                    return false; // More than one difference
                diff_found = true;
                j++; // Skip the extra character in longer string
            }
            else
            {
                i++;
                j++;
            }
        }

        return true;
    }
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
    // Quick validation checks
    if (begin_word == end_word)
        return {begin_word};

    if (abs((int)begin_word.length() - (int)end_word.length()) > 1)
    {
        error(begin_word, end_word, "similar length");
        return {};
    }

    if (word_list.find(end_word) == word_list.end())
    {
        error(begin_word, end_word, "valid dictionary");
        return {};
    }

    // Use unordered_set for faster lookups
    unordered_set<string> visited;
    queue<vector<string>> ladders;

    // Track parent words for efficient path reconstruction
    map<string, string> parent;

    ladders.push({begin_word});
    visited.insert(begin_word);

    // Bidirectional search optimization - create word buckets by length
    map<int, vector<string>> words_by_length;
    for (const auto &word : word_list)
        words_by_length[word.length()].push_back(word);

    while (!ladders.empty())
    {
        vector<string> current_ladder = ladders.front();
        ladders.pop();

        string current_word = current_ladder.back();

        // Only consider words of similar length (current length or ±1)
        const vector<int> potential_lengths = {
            (int)current_word.length() - 1,
            (int)current_word.length(),
            (int)current_word.length() + 1};

        for (int len : potential_lengths)
        {
            if (len < 1)
                continue;

            for (const string &word : words_by_length[len])
            {
                if (visited.find(word) == visited.end() && is_adjacent(current_word, word))
                {
                    if (word == end_word)
                    {
                        // Found the target word
                        vector<string> result = current_ladder;
                        result.push_back(word);
                        return result;
                    }

                    vector<string> new_ladder = current_ladder;
                    new_ladder.push_back(word);
                    visited.insert(word);
                    ladders.push(new_ladder);
                }
            }
        }
    }

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
    for (const auto &word : ladder)
    {
        cout << word << " ";
    }
    cout << endl;
}

void verify_word_ladder()
{
    set<string> word_list;
    load_words(word_list, "words.txt");

    string begin_word, end_word;
    cout << "Enter the beginning word: ";
    cin >> begin_word;

    cout << "Enter the ending word: ";
    cin >> end_word;

    vector<string> ladder = generate_word_ladder(begin_word, end_word, word_list);
    print_word_ladder(ladder);
}
