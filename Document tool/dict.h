// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class Dict {
private:
    // You can add attributes/helper functions here
    class SymNode {
    public:
    string key;
    int height;
    int count=1;
    
    SymNode* par = NULL;
    SymNode* left = NULL;
    SymNode* right = NULL;

    SymNode(){
    height=0;
    key="";
    count=1;
    }

    SymNode(string k){
    key=k;
    height=0;
    count=1;
    }

    SymNode*  LeftLeftRotation(){
    if(this->par){
        if(((this->par)->right)==this){
        (this->right)->par=this->par;
        (this->par)->right=this->right;
        this->par=(this->right);
        this->right=((this->par)->left);
        if((this->par)->left){
        ((this->par)->left)->par=this;
        }
        (this->par)->left=this;
    }else{
        (this->right)->par=this->par;
        (this->par)->left=this->right;
        this->par=(this->right);
        this->right=((this->par)->left);
        if((this->par)->left){
        ((this->par)->left)->par=this;
        }
        (this->par)->left=this;
    }
  }else{
        (this->right)->par=NULL;
        this->par=(this->right);
        this->right=((this->par)->left);
        if((this->par)->left){
        ((this->par)->left)->par=this;
        }
        (this->par)->left=this;
  }
  return this->par;
}

SymNode*  RightRightRotation(){
    if(this->par){
    if(((this->par)->left)==this){
        (this->left)->par=this->par;
        (this->par)->left=this->left;
        this->par=(this->left);
        this->left=((this->par)->right);
        if((this->par)->right){
        ((this->par)->right)->par=this;
        }
        (this->par)->right=this;
    }else{
        (this->left)->par=this->par;
        (this->par)->right=this->left;
        this->par=(this->left);
        this->left=((this->par)->right);
        if((this->par)->right){
        ((this->par)->right)->par=this;
        }
        (this->par)->right=this;
    }
  }else{
        (this->left)->par=NULL;
        this->par=(this->left);
        this->left=((this->par)->right);
        if((this->par)->right){
        ((this->par)->right)->par=this;
        }
        (this->par)->right=this;
  }
  return this->par;
}

SymNode*  LeftRightRotation(){
    right->RightRightRotation();
    return this->LeftLeftRotation();
}

  SymNode*  RightLeftRotation(){
    left->LeftLeftRotation();
    return this->RightRightRotation();
}

   ~SymNode(){
  left=NULL;
  right=NULL;
  par=NULL;
   }
    };
  class SymbolTable {
private:
    int size = 0;
    SymNode* root = NULL;

public: 

     SymbolTable(){
  root=NULL;
}

int bal(SymNode* root){
   int hl=0;
   int hr=0;
   if(root->right){
      hr=(root->right)->height;
   }else{
      hr=-1;
   }

   if(root->left){
      hl=(root->left)->height;
   }else{
      hl=-1;
   }

   return (hl-hr);
}

void ht(SymNode* temp){
     int hr=0;
     int hl=0;
     if(!temp){
      return;
     }
     if(temp->right){
        hr=(temp->right)->height;
     }else{
        hr=-1;
     }
     if(temp->left){
        hl=(temp->left)->height;
     }else{
        hl=-1;
     }
     
     if(hr>hl){
        temp->height=hr+1;
     }else{
        temp->height=hl+1;
     }
}


void  insert(string k){
  if(size==0){
      root=new SymNode();
      root->key=k;
      size++;
      return;
  }
  SymNode* temp=root;
  SymNode* parent=NULL;
  while(temp!=NULL){
     parent=temp;
     if(temp->key>k){
        temp=temp->left;
     }else{
        temp=temp->right;
     }   
  }
  temp=new SymNode(k);
  size++;
  temp->par=parent;
  if(parent){
    if(temp->key<parent->key){
        parent->left=temp;
    }else{
        parent->right=temp;
    }
  }
  bool f=false;
  while(temp!=NULL){
     if(temp==root){
        f=true;
     }
     if(bal(temp)<-1){
       if(bal(temp->right)>0){
        temp=temp->LeftRightRotation();
       }else{
        temp=temp->LeftLeftRotation();
       }
     }
     else if(bal(temp)>1){
      if(bal(temp->left)<0){
        temp=temp->RightLeftRotation();
      }else{
        temp=temp->RightRightRotation();
      }
     }
     ht(temp->left);
     ht(temp->right);
     ht(temp);
     if(f){
        root=temp;
     }
     temp=temp->par;
  }

}

SymNode* min(SymNode* root){
    while(root->left!=NULL){
        root=root->left;
    }
    return root;
}

void  remove(string k){
  SymNode* temp=root;
  while(temp!=NULL && temp->key!=k){
      if(k>temp->key){
        temp=temp->right;
      }else{
        temp=temp->left;
      }
  }
  if(temp==NULL){
    return;
  }else{
      SymNode* ans=NULL;
      if(temp->left){
        if(temp->right){
            SymNode* min1=min(temp->right);
            temp->key=min1->key;
            temp->count=min1->count;
            root=temp->right;
            remove(min1->key);
        }else{
            if(!(temp->par)){
            root=temp->left;
            (root)->par=NULL;
            delete temp;
              }else{
            ans=temp->left;
            if((temp->par)->left==temp){
                (temp->left)->par=temp->par;
                (temp->par)->left=temp->left;
                delete temp;
            }else{
                (temp->left)->par=temp->par;
                (temp->par)->right=temp->left;
                delete temp;
            }
            
        }
        size--;


        }

      }else if(temp->right){
        if(!temp->par){
            root=temp->right;
            (root)->par=NULL;
            delete temp;
        }else{
            ans=temp->right;
            if((temp->par)->left==temp){
                (temp->right)->par=temp->par;
                (temp->par)->left=temp->right;
                delete temp;
            }else{
                (temp->right)->par=temp->par;
                (temp->par)->right=temp->right;
                delete temp;
            }
        }
        size--;
      }else{
        if(!(temp->par)){
            delete temp;
            root=NULL;
        }else{
        ans=temp->par;
        if((temp->par)->left==temp){
          (temp->par)->left=NULL;
        }else{
          (temp->par)->right=NULL;
        }
        delete temp;
        }
        size--;
      }
      if(ans){
        bool f=false;
    while(ans!=NULL){
     if(!ans->par){
        f=true;
     }
     if(bal(ans)<-1){
       if(bal(ans->right)>0){
        ans=ans->LeftRightRotation();
       }else{
        ans=ans->LeftLeftRotation();
       }
     }
     else if(bal(ans)>1){
      if(bal(ans->left)<0){
        ans=ans->RightLeftRotation();
      }else{
        ans=ans->RightRightRotation();
      }
     }
     ht(ans->left);
     ht(ans->right);
     ht(ans);
     if(f){
        root=ans;
     }
     ans=ans->par;
  }

      }


      }
  }

int  search(string k){
  SymNode* temp=root;
  while(temp!=NULL && temp->key!=k){
      if(k>temp->key){
        temp=temp->right;
      }else{
        temp=temp->left;
      }
  }
  if(temp==NULL){
    return 0;
  }else{
    return temp->count;
  }
}

void  assign_count(string k){
  SymNode* temp=root;
  while(temp!=NULL && temp->key!=k){
      if(k>temp->key){
        temp=temp->right;
      }else{
        temp=temp->left;
      }
  }
  if(temp==NULL){
    insert(k);
    return;
  }else{
    temp->count++;
  }
}

int  get_size(){
    return size;
}

SymNode*  get_root(){
    return root;

}

void deleteTree(SymNode* root) {
    if (root==NULL) {
        return;
    }
    
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}
void dump(SymNode* temp,ofstream &MyFile){
    if(!temp){
        return;
    }
    
    string s=temp->key + ", " + to_string(temp->count);
    MyFile <<s<<endl;
    dump(temp->left, MyFile);
    dump(temp->right, MyFile);
}

 ~SymbolTable(){
  deleteTree(root);
  root=NULL;
}
};

    class Chaining{
      public:
       vector<SymbolTable> store;
       int size=0;
       Chaining(){
        size=0;
        store.resize(75967);
       }
       int hash(std::string id) {
      // IMPLEMENT YOUR CODE HERE
       int s1=0;
       int i=1;
       for(char s : id){
         s1+= i*int(s-' ');
         s1=s1%75967;
         i=2*(i%75967);
       }
       return s1; // Placeholder return value
     }
     int get_count(string k){
        int x=hash(k);
        return store[x].search(k);
     } 
     void ins(string k){
       int x=hash(k);
       store[x].assign_count(k);
     }

     void dumper(string filename){
        ofstream MyFile(filename,ios::app);
        if (!MyFile.is_open()) {
        // If the file is not open, open it in append mode
        MyFile.open(filename, ios::app);
        }
        for(int i=0;i<75967;i++){
            store[i].dump(store[i].get_root(),MyFile);
        }
        MyFile.close();
     }
    };

    Chaining  c;
    bool check(char k){
    string seperators=" .,-:!\"\'()?—[]“”‘’˙;@";
    for(char a:seperators){
        if(k==a){
            return true;
        }
    }
    return false;
    }
    

public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    int get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
};