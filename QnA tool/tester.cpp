// // #include<bits/stdc++.h>
// #include "Node.h"
// #include "qna_tool.h"

// using namespace std;

// #define FILENAME "mahatma-gandhi-collected-works-volume-1.txt"

// int main(){
//     QNA_tool qna_tool;
//     qna_tool.query("What is the purpose of life?", "answer.txt");
//     return 0;
// }
// // // #include<bits/stdc++.h>
// // #include "Node.h"
// // #include "qna_tool.h"

// // using namespace std;

// // #define FILENAME "mahatma-gandhi-collected-works-volume-1.txt"

// // int main(){
// //     QNA_tool qna_tool;
// //     qna_tool.query("What is the purpose of life?", "answer.txt");
// //     return 0;
// // }
#include <sstream>
#include <iostream>
#include <string>
#include "Node.h"
#include "qna_tool.h"

using namespace std;

int main(){

    QNA_tool *qna_tool = new QNA_tool();

    int num_books = 98;

    for(int i = 1; i <= num_books; i++){

        std::cout << "Inserting book " << i << std::endl;

        std::string filename = "mahatma-gandhi-collected-works-volume-";
        filename += to_string(i);
        filename += ".txt";

        std::ifstream inputFile(filename);

        if (!inputFile.is_open()) {
            std::cerr << "Error: Unable to open the input file mahatma-gandhi." << std::endl;
            return 1;
        }

        std::string tuple;
        std::string sentence;


        while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
            // Get a line in the sentence
            tuple += ')';

            std::vector<int> metadata;    
            std::istringstream iss(tuple);

            // Temporary variables for parsing
            std::string token;

            // Ignore the first character (the opening parenthesis)
            iss.ignore(1);

            // Parse and convert the elements to integers
            while (std::getline(iss, token, ',')) {
                // Trim leading and trailing white spaces
                size_t start = token.find_first_not_of(" ");
                size_t end = token.find_last_not_of(" ");
                if (start != std::string::npos && end != std::string::npos) {
                    token = token.substr(start, end - start + 1);
                }
                
                // Check if the element is a number or a string
                if (token[0] == '\'') {
                    // Remove the single quotes and convert to integer
                    int num = std::stoi(token.substr(1, token.length() - 2));
                    metadata.push_back(num);
                } else {
                    // Convert the element to integer
                    int num = std::stoi(token);
                    metadata.push_back(num);
                }
            }
            qna_tool->insert_sentence(metadata[0], metadata[1], metadata[2], metadata[3], sentence);
        }

        inputFile.close();

    }
    while(true){
        string question;
        cout << "Query: ";
        getline(cin,question);
        if(question=="quit"){
            break;
        }
        qna_tool->query(question, "gpt_call.py");  
        cout << endl;  
    }

    
    return 0;
}