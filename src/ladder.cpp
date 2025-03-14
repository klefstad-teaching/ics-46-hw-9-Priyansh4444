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
vector<string> generate_word_ladder(const string &begin_word, const string &end_word, const set<string> &word_list)
{
    // Quick validations
    if (begin_word == end_word)
    {
        return {};
    }

    if (word_list.find(end_word) == word_list.end())
    {
        error(begin_word, end_word, "valid dictionary");
        return {};
    }

    if (begin_word.length() != end_word.length())
    {
        error(begin_word, end_word, "same length");
        return {};
    }

    // Using bidirectional BFS to improve efficiency (From the neetcode link professor showed us!!!)
    set<string> word_set(word_list.begin(), word_list.end());

    // Parent maps for reconstructing the path
    unordered_map<string, string> parent_begin;
    unordered_map<string, string> parent_end;

    // Queues for BFS from both directions
    queue<string> queue_begin;
    queue<string> queue_end;

    queue_begin.push(begin_word);
    queue_end.push(end_word);

    parent_begin[begin_word] = "";
    parent_end[end_word] = "";

    string meeting_point = "";
    bool break_loop = false;
    while (!queue_begin.empty() && !queue_end.empty())
    {
        // Process the smaller queue first for efficiency
        if (queue_begin.size() > queue_end.size())
        {
            swap(queue_begin, queue_end);
            swap(parent_begin, parent_end);
        }

        int size = queue_begin.size();
        for (int i = 0; i < size; i++)
        {
            string current = queue_begin.front();
            queue_begin.pop();

            // Try changing each character
            string temp = current;
            for (int j = 0; j < current.length(); j++)
            {
                char original = temp[j];

                // Try all possible characters
                for (char c = 'a'; c <= 'z'; c++)
                {
                    if (c == original)
                        continue;

                    temp[j] = c;

                    // Check if we've found a meeting point
                    if (parent_end.find(temp) != parent_end.end())
                    {
                        meeting_point = temp;
                        parent_begin[meeting_point] = current;
                        break_loop = true;
                    }

                    // If this is a valid word and we haven't visited it yet
                    if (word_set.find(temp) != word_set.end() &&
                        parent_begin.find(temp) == parent_begin.end())
                    {
                        parent_begin[temp] = current;
                        queue_begin.push(temp);
                    }
                    if (break_loop)
                        break;
                }
                if (break_loop)
                    break;
                temp[j] = original; // Restore the original character
            }
        }
        if (break_loop)
            break;
    }

    if (meeting_point.empty())
    {
        error(begin_word, end_word, "connected");
        return {};
    }

    vector<string> path_begin;
    string current = meeting_point;

    // Build path from begin_word to meeting_point
    while (!current.empty())
    {
        path_begin.push_back(current);
        current = parent_begin[current];
    }

    // Reverse to get the correct order
    reverse(path_begin.begin(), path_begin.end());

    // Build path from meeting_point to end_word
    vector<string> path_end;
    current = parent_end[meeting_point];

    while (!current.empty())
    {
        path_end.push_back(current);
        current = parent_end[current];
    }

    vector<string> complete_path = path_begin;
    complete_path.insert(complete_path.end(), path_end.begin(), path_end.end());

    return complete_path;
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