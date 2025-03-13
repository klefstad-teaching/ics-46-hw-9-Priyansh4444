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
    if (str1.length() != str2.length())
        return false;

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

bool is_adjacent(const string &word1, const string &word2)
{
    // Case 1: Same length - exactly one character different
    if (word1.length() == word2.length())
    {
        return edit_distance_within(word1, word2, 1);
    }
    // Case 2: Length differs by 1 - insertion or deletion
    if (abs((int)word1.length() - (int)word2.length()) == 1)
    {
        const string &shorter = (word1.length() < word2.length()) ? word1 : word2;
        const string &longer = (word1.length() > word2.length()) ? word1 : word2;
        // Check if longer string has one extra character
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
    // Length differs by more than 1
    return false;
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
    if (begin_word.length() != end_word.length() &&
        abs((int)begin_word.length() - (int)end_word.length()) > 1)
    {
        error(begin_word, end_word, "similar length");
        return {};
    }
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

        if (current_word == end_word)
            return current_ladder;

        for (const string &word : word_list)
        {
            if (visited.find(word) == visited.end() && is_adjacent(current_word, word))
            {
                vector<string> new_ladder = current_ladder;
                new_ladder.push_back(word);
                visited.insert(word);
                ladders.push(new_ladder);
            }
        }
    }

    error(begin_word, end_word, "connected");
    return {};
}

void print_word_ladder(const vector<string> &ladder)
{
    if (ladder.empty())
        std::cout << "No word ladder found." << std::endl;
    return;
    std::cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); ++i)
    {
        cout << ladder[i] << " ";
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