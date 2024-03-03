#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class Trie{
public:
    class Nde{
    public:
        char c;
        long long int count;
        // long long int child_count;
        bool endofWord;
        Trie *child;

        Nde(){
            // child_count = 0;
            count = 0;
            child = nullptr;
        }
        Nde(char letter){
            c = letter;
            count = 0;
            child = new Trie();
        }
    };

    vector<Nde *> * TrieVector;
    int t_size = 0;
    Trie(){
        TrieVector = new vector<Nde*>;
        t_size = 0;
    }

    void insert(string &word, long long int x = 1){
        // cout << word << endl;
        // cout << this->TrieVector->size() << endl;
        int i = 0;
        Trie *temp = this;
        while (i < word.length()){
            // cout << i << " " << word.length() << endl;
            if (i == word.length() - 1){
                char tempChar = tolower(word.at(i));
                bool found = 0;
                int j = 0;
                while (j < temp->t_size){
                    if (temp->TrieVector->at(j)->c == tempChar){
                        // temp->TrieVector->at(j).child_count +=x;
                        found = 1;
                        break;
                    }
                    else{
                        j++;
                    }
                }
                if (found){
                    // if(word=="mahatma"||word=="MAHATMA"||word=="Mahatma"){
                    //     cout << word << " " << temp->TrieVector->at(j).count << endl;
                    // }
                    temp->TrieVector->at(j)->count += x;
                    i++;
                    break;
                }
                else{
                    Nde * n = new Nde(tempChar);
                    n->count = x;
                    temp->TrieVector->push_back(n);
                    temp->t_size++;
                    break;
                    i++;
                }
            }
            else{
                char tempChar = tolower(word.at(i));
                bool found = 0;
                int j = 0;
                // cout << "hels" << endl;
                // cout << (temp==this) << endl;
                // cout << this->t_size << endl;
                // cout << j << " " << temp->t_size << endl;
                while (j < temp->t_size){
                    if (temp->TrieVector->at(j)->c == tempChar){
                        // temp->TrieVector->at(j).child_count +=x;
                        found = 1;
                        break;
                    }
                    else{
                        j++;
                    }
                }
                if (found){
                    temp = temp->TrieVector->at(j)->child;
                    i++;
                }
                else{
                    // cout << "sda" << endl;
                    Nde *n = new Nde(tempChar);
                    temp->TrieVector->push_back(n);
                    temp->t_size++;
                    temp = n->child;
                    i++;
                }
            }
        }
    }

    long long int getCount(string word){
        int i = 0;
        Trie *temp = this;
        while (i < word.length()){
            // cout << i << " " << word.length() << endl;
            if (i == word.length() - 1){
                char tempChar = word.at(i);
                bool found = 0;
                int j = 0;
                while (j < temp->t_size){
                    if (temp->TrieVector->at(j)->c == tolower(tempChar)){
                        found = 1;
                        break;
                    }
                    else{
                        j++;
                    }
                }
                if(found){
                    return temp->TrieVector->at(j)->count;
                }
                else{
                    return 0;
                }
            }
            else{
                char tempChar = word.at(i);
                bool found = 0;
                int j = 0;
                // cout << "hel" << endl;
                while (j < temp->t_size){
                    // cout << temp->t_size << endl;
                    // cout << j << " " << temp->TrieVector->size() << endl;
                    if (temp->TrieVector->at(j)->c == tolower(tempChar)){
                        found = 1;
                        break;
                    }
                    else{
                        j++;
                    }
                }
                // cout << "hel2" << endl;
                if (found){
                    temp = temp->TrieVector->at(j)->child;
                    i++;
                }
                else{
                    return 0;
                }
            }
        }
        return 0;
    }
};

class Dict{
private:
    Trie *dictionary;

public:
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    long long int get_word_count(string word);

    void dump_dictionary(string filename);
};

class Paragraph{
public:
    // static vector<Paragraph *> list;
    int para_num;
    int book_code;
    int page_num;
    Trie *para_trie;
    // Trie * new_para_trie;
    Paragraph(int para_num, int book_code, int page_num){
        this->page_num = page_num;
        this->book_code = book_code;
        this->para_num = para_num;
        para_trie = new Trie();
        // new_para_trie = new Trie();
        // list.push_back(this);
    }
};
struct info{
        int book;
        int page;
        int para;
        long double score;
};
class Maxheap{
    public:
    vector<info> heap;
        Maxheap(){}
        ~Maxheap(){}
        void buildheap(vector<info>& arr){
            int n=arr.size();
            for(int i=0;i<n;i++){
                heap.push_back(arr[i]);
            }
            for(int i=(n/2)-1;i>=0;i--){
                int ind=i;
                int maxi=i;
                while(true){
                int left=2*ind+1;
                int right=2*ind+2;
                if((left <n) &&(heap[ind].score<=heap[left].score)){
                        maxi=left;
                } 
                if((right<n)&&(heap[maxi].score<=heap[right].score)){
                        maxi=right;
                }
                if(maxi==ind){break;}
                info temp=heap[ind];
                heap[ind]=heap[maxi];
                heap[maxi]=temp;
                ind=maxi;
                }
            }
        }
        void insert (info node){
            heap.push_back(node);
            int s=heap.size()-1;
            while(true){
                int parent=(s-1)/2;
                if(parent==0||heap[parent].score>=heap[s].score){break;}
                info temp =heap[s];
                heap[s]=heap[parent];
                heap[parent]=temp;
                s=parent;
            }
        }
        info del(){
            info t = heap[0];
            heap[0]=heap.back();
            heap.pop_back();
            int n=heap.size();
            int ind=0;
            int maxi=0;
            while(true){
                int left=2*ind+1;
                int right=2*ind+2;
                if((left <n) &&(heap[ind].score<=heap[left].score)){
                    maxi=left;
                } 
                if((right<n)&&(heap[maxi].score<=heap[right].score)){
                    maxi=right;
                }
                if(maxi==ind){break;}
                info temp=heap[ind];
                heap[ind]=heap[maxi];
                heap[maxi]=temp;
                ind=maxi;
            }
            return t;
        }
};