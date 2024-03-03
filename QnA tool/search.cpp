 // Do NOT add any other includes
#include "search.h"

int * REQD(string &pattern){
    int*rqd = new int[pattern.size()];
    int curr = 0; 
    rqd[0] = 0;
    int i = 1;
    while (i < pattern.size()) {
        if(pattern[i] == pattern[curr]){
            curr++;
            rqd[i] = curr;
            i++;
        }
        else{
            if (curr != 0) {
                curr = rqd[curr - 1]; 
            }
            else{
                rqd[i] = 0;
                i++;
            }
        }
    }
    return rqd;
}

SearchEngine::SearchEngine(){
    // Implement your function here
    string_database = new vector<string>;
    corresponding_database = new vector<Node*>;  
}

SearchEngine::~SearchEngine(){
    // Implement your function here  
    delete string_database;
    delete corresponding_database;
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here  
    string_database->push_back(sentence);
    Node * newInsert = new Node(book_code,page,paragraph,sentence_no,0);
    corresponding_database->push_back(newInsert);
}

Node* SearchEngine::search(string pattern, int& n_matches){
    // Implement your function here
    n_matches = 0;
    Node*head=nullptr;
    Node*recent;
    int off = 0;
    int * to_match_arr = REQD(pattern);
    for(int x=0;x<string_database->size();x++){
        int i=0;
        int j=-1;
        bool check = false;
        vector<int> o;
        while(true){
            if(j==pattern.size()-1){
                off = i-pattern.size();
                check = true;
                o.push_back(off);
                j = to_match_arr[j]-1;
            }
            if(i>=string_database->at(x).size()){
                break;
            }
            if((char)tolower(string_database->at(x)[i])==(char)tolower(pattern[j+1])){
                i++;
                j++;
            }
            else{
                if(j==-1){
                    i++;
                }
                else{
                    j = to_match_arr[j]-1;
                }
            }
        }
        if(check){
            corresponding_database->at(x)->offset = o[0];
            if(n_matches==0){
                head = corresponding_database->at(x);
            }
            else{
                recent->right = corresponding_database->at(x);
                corresponding_database->at(x)->left = recent;
            }
            recent = corresponding_database->at(x);
            n_matches++;
        }
        for(int k=1;k<o.size();k++){
            Node * m = new Node(corresponding_database->at(x)->book_code,corresponding_database->at(x)->page,corresponding_database->at(x)->paragraph,corresponding_database->at(x)->sentence_no,o[k]);
            recent->right = m;
            m->left = recent;        
            n_matches++;
            recent = m;            
        }
    } 
    return head;
}
void checker(SearchEngine &se,string &s){
    int x;
    Node * ans = se.search(s,x);
    cout << x << endl;
    for(int i=0;i<x;i++){
        cout << ans->book_code << " " << ans->page << " " << ans->paragraph << " " << ans->sentence_no << " " << ans->offset << endl;
        ans = ans->right;        
    }
}

// int main(){
//     SearchEngine se;
//     se.insert_sentence(1,2,3,4,"a b a b a b");
//     // se.insert_sentence(12,5,2,8,"I am Madhav");
//     checker(se,"a b a");
//     return 0;
// }