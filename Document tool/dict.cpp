// Do NOT add any other includes
#include "dict.h"

Dict::Dict(){
    // Implement your function here    
}

Dict::~Dict(){
    // Implement your function here    
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here 
    string word=""; 
    for(int i=0;i<sentence.length();i++){
        if(sentence[i]>='A' && sentence[i]<='Z'){
            sentence[i]=char(sentence[i]-'A'+'a');
        }
        if(check(sentence[i])){
         if(word.length()>0){
          c.ins(word);
         }
         word=""; 
        }else{
         word=word+sentence[i];
        }
    }
    
    if(word.length()!=0){
        c.ins(word);
    }
    return;
}



int Dict::get_word_count(string word){
    // Implement your function here
    return c.get_count(word);  
}

void Dict::dump_dictionary(string filename){
    // Implement your function here  
    c.dumper(filename);
    return;
}