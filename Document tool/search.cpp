// Do NOT add any other includes
#include "search.h"
SearchEngine::SearchEngine(){
    // Implement your function here  
}

SearchEngine::~SearchEngine(){
    // Implement your function here  
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here
    header.push_back(vector<int>{book_code,page,paragraph,sentence_no});
    for(int i=0;i<sentence.length();i++){
        if(sentence[i]>='A' && sentence[i]<='Z'){
            sentence[i]=char(sentence[i]-'A'+'a');
        }
    }
    v.push_back(sentence);  
    return;
}

Node* SearchEngine::search(string pattern, int& n_matches){
    // Implement your function here
    for(int k=0;k<pattern.length();k++){
        //to make the pattern of small size
        if(pattern[k]>='A' && pattern[k]<='Z'){
            pattern[k]=char(pattern[k]-'A'+'a');
        }
    }
    int p=pattern.length();
    
    Node* head=new Node(-1,-1,-1,-1,-1);
    head->left=NULL;
    head->right=NULL;
    Node* temp=head;
    n_matches=0;
    int hpat=0;
    int pow=1;
    for(int i=0;i<p-1;i++){
        pow=(pow*256)%127;
    }
    for(int i=0;i<p;i++){
        hpat=(256*hpat + pattern[i])%127;
    }

    for(int i=0;i<v.size();i++){
        int h=0;
        int n=v[i].length();    
        if(n>=p){
            for(int j=0;j<p;j++){
            h=(256*h + v[i][j])%127;
            }
            
            int off=0;
            for(int j=0;j<=n-p;j++){
                
                if(hpat==h){
                    int k=0;
                    for(k=0;k<p;k++){
                        if(v[i][k+j]!=pattern[k]){
                            break;
                        }
                    }
                    if(k==p){
                    temp->right=new Node(header[i][0],header[i][1],header[i][2],header[i][3],j);
                    temp->right->left=temp;
                    temp=temp->right;
                    n_matches++;
                    }
                    
                }
                // if(j+pattern.size()<v[i].size){
                    
                // h=(((((h)%127)*((256)%127))%127)-((((k)%127)*((pow)%127))%127)+k2)%127;
                // }
                if(j<n-p){
                    h=(256*(h-v[i][j]*pow)+v[i][j+p])%127;
                    if(h<0){
                        h=h+127;
                    }
                }
        }
        }
    }
    if(head->right){
        temp->right=NULL;
        temp=head->right;
        temp->left=NULL;
        head->right=NULL;
        delete head;
        head=NULL;
    }else{
        temp=NULL;
        delete head;
        head=NULL;
    }
    return temp;
}