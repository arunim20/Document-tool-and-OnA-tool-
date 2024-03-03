#include <iostream>
#include <fstream>
#include "Node.h"
#include "dict.h"
#include "search.h"
using namespace std;

class QNA_tool {

private:
    Node *get_new_top_k_para(string question, int k);
    void query_llm(string filename, Node* root, int k, string API_KEY, string question); 
    vector<string> remove1 = {"a", "about", "above", "absolute", "after", "again", "all", "almost", "already", "also", "always", "am", "an", "and", "another", "any", "are", "aren't", "as", "at", "be", "because", "been", "before", "being", "below", "between", "both", "but", "by", "can", "complete", "could", "did", "do", "does", "down", "during", "each", "enough", "entire", "etc", "for", "from", "good", "great", "had", "has", "have", "he", "her", "here", "hers", "herself", "him", "himself", "his", "how", "however", "i", "if", "in", "into", "is", "it", "just", "later", "least", "less", "let", "many", "me", "most", "mostly", "much", "must", "my", "never", "no", "of", "oh", "on", "once", "only", "or", "other", "our", "shall", "she", "should", "so", "some", "still", "such", "than", "that", "the", "their", "them", "then", "thence", "there", "these", "they", "this", "those", "till", "to", "too", "under", "unless", "until", "up", "usually", "various", "very", "was", "we", "were", "what", "whatever", "when", "whenever", "where", "where's", "wherever", "which", "whichever", "while", "who", "why", "will", "with", "would", "you", "your"};
    vector<string> remove2 =  {"Absolutely stunning!", "Aha!", "Amazing!", "Astonishing!", "Awesome!", "Bingo!", "Bravo!", "Cheers!", "Congratulations!", "Eureka!", "Excellent!", "Fantastic!", "Fascinating!", "Good job!", "Great!", "Hallelujah!", "Hooray!", "Hurrah!", "Impressive work!", "Impressive!", "Incredible!", "Just fantastic!", "Marvelous!", "Oh dear!", "Oh my goodness!", "Oh my gosh!", "Oh no!", "Oh well!", "Oh wow!", "Oh yes!", "Outstanding!", "Phew!", "Simply superb!", "So happy!", "Spectacular!", "Super!", "Superb!", "Terrific!", "That's incredible!", "This is amazing!", "Unbelievable!", "Well done!", "Wonderful!", "Wow!", "Yay!", "Yippee!", "You did it!", "a", "about", "above", "absolute", "absolutely", "after", "again", "against", "ain't", "all", "almost", "already", "also", "although", "always", "am", "an", "and", "another", "any", "anybody", "anyhow", "anything", "anytime", "anyway", "are", "aren't", "as", "at", "be", "because", "been", "before", "being", "below", "between", "bit", "blah", "blah blah", "both", "but", "by", "can", "can't", "cannot", "complete", "completely", "could", "couldn't", "did", "didn't", "do", "does", "doesn't", "doing", "don't", "down", "during", "each", "enough", "entire", "entirely", "et cetera", "etc", "etcetera", "ever", "every", "everybody", "everyone", "everything", "everywhere", "few", "for", "frequently", "from", "further", "had", "hadn't", "has", "hasn't", "have", "haven't", "having", "he", "he'd", "he'll", "he's", "her", "here", "here's", "hereafter", "hers", "herself", "him", "himself", "his", "how", "how's", "however", "i", "i'd", "i'll", "i'm", "i've", "if", "in", "indeed", "into", "is", "isn't", "it", "it's", "its", "itself", "just", "later", "least", "less", "let's", "little", "many", "may", "me", "meanwhile", "might", "more", "moreover", "most", "mostly", "much", "must", "mustn't", "my", "myself", "never", "no", "nobody", "none", "nor", "not", "nothing", "now", "nowadays", "nowhere", "o", "of", "off", "often", "on", "once", "only", "or", "other", "otherwise", "ought", "our", "ours", "ourselves", "out", "over", "own", "partially", "partly", "quickly", "rarely", "rather", "regardless", "same", "seldom", "several", "shall", "shan't", "she", "she'd", "she'll", "she's", "should", "shouldn't", "simply", "since", "slowly", "so", "some", "somebody", "someday", "somehow", "something", "sometimes", "somewhere", "sooner", "still", "stuff", "such", "than", "that", "that's", "the", "their", "theirs", "them", "themselves", "then", "thence", "there", "there's", "thereafter", "therefore", "these", "they", "they'd", "they'll", "they're", "they've", "thing", "this", "those", "though", "through", "till", "to", "too", "under", "unless", "until", "up", "usually", "various", "very", "was", "wasn't", "we", "we'd", "we'll", "we're", "we've", "well", "were", "weren't", "what", "what's", "whatever", "when", "when's", "whenever", "where", "where's", "wherever", "which", "whichever", "while", "who", "who's", "whoever", "whom", "why", "why's", "will", "with", "won't", "would", "wouldn't", "you", "you'd", "you'll", "you're", "you've", "your", "yours", "yourself", "yourselves" };
    vector<Paragraph*> list; 
    Trie * main_Trie;
    Trie * corpus_Trie;  

    /* ------------------------------------------- */
public:
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    Node* get_top_k_para(string question, int k);

    std::string get_paragraph(int book_code, int page, int paragraph);

    void query(string question, string filename);
};