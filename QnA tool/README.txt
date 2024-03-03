COL106 Assignment 7
-------------------------
Libraries to be Included for ChatGPT:
sudo apt install python3-pip             
pip install openai                 
pip show openai        (to confirm the installation)
------------------------
Libraries to be Included for HugChat:

------------------------
Compilation and Running Details 
---------------------------------
To compile your program, use the following commands: 
$ (LINUX)   g++ tester.cpp node.cpp dict.cpp search.cpp qna_tool.cpp -o m.out
$ (WINDOWS) g++ tester.cpp node.cpp dict.cpp search.cpp qna_tool.cpp -o m.exe

To execute:
$ (LINUX)   ./m.out 
$ (WINDOWS) m.exe
You must provide an input which would be a query to be processed by the code. The answer to the query would be returned on the terminal and will also be printed in the query.txt file. The code will keep asking for queries until you input “quit”.

$ ./m.out
>> Query:
$ Who was Mahatma Gandhi?
>> .....answer would be printed....
.
.
continue for further queries.
.
.
>> Query:
$ quit
The program terminates after this step.