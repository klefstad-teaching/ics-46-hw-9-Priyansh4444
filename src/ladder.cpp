#include "ladder.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Error-reporting function.
void error(string word1, string word2, string msg)
{
    cerr << "Error: " << word1 << " and " << word2 << " are not " << msg << " words" << endl;
}

// (Legacy) edit_distance_within function kept for reference.
bool edit_distance_within(const std::string &str1, const std::string &str2, int d)
{
    if (abs((int)str1.length() - (int)str2.length()) > d)
        return false;
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
    const string &shorter = (str1.length() < str2.length()) ? str1 : str2;
    const string &longer = (str1.length() > str2.length()) ? str1 : str2;
    size_t i = 0, j = 0;
    int edits = 0;
    while (i < shorter.length() && j < longer.length())
    {
        if (shorter[i] != longer[j])
        {
            edits++;
            if (edits > d)
                return false;
            j++; // skip char in longer string
        }
        else
        {
            i++;
            j++;
        }
    }
    edits += longer.length() - j;
    return edits <= d;
}

// (Legacy) is_adjacent function kept for reference.
bool is_adjacent(const string &word1, const string &word2)
{
    if (word1 == word2)
        return true;
    if (abs((int)word1.length() - (int)word2.length()) > 1)
        return false;
    if (word1.length() == word2.length())
    {
        int diffs = 0;
        for (size_t i = 0; i < word1.length(); i++)
        {
            if (word1[i] != word2[i])
                diffs++;
            if (diffs > 1)
                return false;
        }
        return diffs == 1;
    }
    const string &shorter = (word1.length() < word2.length()) ? word1 : word2;
    const string &longer = (word1.length() > word2.length()) ? word1 : word2;
    size_t i = 0, j = 0;
    bool found_diff = false;
    while (i < shorter.length() && j < longer.length())
    {
        if (shorter[i] != longer[j])
        {
            if (found_diff)
                return false;
            found_diff = true;
            j++;
        }
        else
        {
            i++;
            j++;
        }
    }
    return (i == shorter.length() && (j == longer.length() || j == longer.length() - 1)) ||
           (found_diff && i == shorter.length() && j == longer.length());
}

vector<string> generate_neighbors(const string &word, const set<string> &word_list)
{
    vector<string> neighbors;
    for (size_t i = 0; i < word.size(); ++i)
    {
        string candidate = word;
        for (char c = 'a'; c <= 'z'; ++c)
        {
            if (candidate[i] == c)
                continue;
            candidate[i] = c;
            if (word_list.find(candidate) != word_list.end())
                neighbors.push_back(candidate);
        }
    }
    for (size_t i = 0; i <= word.size(); ++i)
    {
        for (char c = 'a'; c <= 'z'; ++c)
        {
            string candidate = word;
            candidate.insert(candidate.begin() + i, c);
            if (word_list.find(candidate) != word_list.end())
                neighbors.push_back(candidate);
        }
    }
    if (word.size() > 1)
    {
        for (size_t i = 0; i < word.size(); ++i)
        {
            string candidate = word;
            candidate.erase(candidate.begin() + i);
            if (word_list.find(candidate) != word_list.end())
                neighbors.push_back(candidate);
        }
    }
    std::sort(neighbors.begin(), neighbors.end());
    return neighbors;
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
    queue<vector<string>> ladder_queue;
    vector<string> initial{begin_word};
    ladder_queue.push(initial);
    if (begin_word == end_word)
        return {};
    set<string> visited;
    visited.insert(begin_word);

    while (!ladder_queue.empty())
    {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        string last_word = ladder.back();

        vector<string> neighbors = generate_neighbors(last_word, word_list);
        for (const auto &neighbor : neighbors)
        {
            if (visited.find(neighbor) != visited.end())
                continue;
            visited.insert(neighbor);
            vector<string> new_ladder = ladder;
            new_ladder.push_back(neighbor);
            if (neighbor == end_word)
                return new_ladder;
            ladder_queue.push(new_ladder);
        }
    }
    return vector<string>();
}

void print_word_ladder(const vector<string> &ladder)
{
    if (ladder.empty())
    {
        cout << "No word ladder found." << endl;
        return;
    }
    cout << "Word ladder found: ";
    for (const auto &word : ladder)
        cout << word << " ";
    cout << endl;
}

// Stub for verifying a word ladder (implementation can be added as needed).
void verify_word_ladder()
{
    return;
}
