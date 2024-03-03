#include "dict.h"
#include <iostream>
using namespace std;

Dict::Dict()
{
}

Dict::~Dict()
{
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    int i = 0;
    int j = 0;
    while (i < sentence.size())
    {
        while (j < sentence.size())
        {
            if ((sentence.at(j) >= char('a')) && (sentence.at(j) <= char('z')))
            {
                j++;
            }
            else if ((sentence.at(j) == ' ') || (sentence.at(j) == '.') || (sentence.at(j) == ',') || (sentence.at(j) == '-') || (sentence.at(j) == ':') || (sentence.at(j) == '!') || (sentence.at(j) == '\"') || (sentence.at(j) == '\'') || (sentence.at(j) == '(') || (sentence.at(j) == ')') || (sentence.at(j) == '?') || (sentence.at(j) == '[') || (sentence.at(j) == ']') || (sentence.at(j) == ';') || (sentence.at(j) == '@'))
            {
                break;
            }
            else
            {
                j++;
            }
        }
        string word = sentence.substr(i, j - i);
        dictionary->insert(word);
        j++;
        i = j;
    }
    return;
}

long long int Dict::get_word_count(string word)
{
    return dictionary->getCount(word);
}

void Dict::dump_dictionary(string filename){
    // vector<keyValPair> words;
    // string temp = "";
    // dfs(dictionary, temp, words);
    // ofstream write_file;
    // write_file.open(filename);
    // for (keyValPair &entry : words)
    // {
    //     write_file << entry.print();
    // }
    // write_file.close();
    return;
}