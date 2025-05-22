#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;

#define FILENAME "mahatma-gandhi-collected-works-volume-1.txt"

#define TRUE 1
#define FALSE 0
class keyValPair{
public:
    string key;
    long long int freq;
    keyValPair(string &k, long long int &f)
    {
        key = k;
        freq = f;
    }
    string print()
    {
        return key + ", " + to_string(freq) + "\n";
    }
};

void dfs(Trie *current, string &prefix, vector<keyValPair> &words){
    if (current->TrieVector->size() < 1)
    {
        return;
    }
    else
    {
        string new_prefix;
        for (int i = 0; i < current->TrieVector->size(); i++)
        {
            new_prefix = prefix + current->TrieVector->at(i)->c;
            if (current->TrieVector->at(i)->count > 0)
            {
                words.push_back(keyValPair(new_prefix, current->TrieVector->at(i)->count));
            }
            dfs(current->TrieVector->at(i)->child, new_prefix, words);
        }
    }
}

int stem(char *p, int index, int position);
static char *b;

static int k;
static int k0;
static int j;

static int isConsonant(int index){
  switch (b[index]) {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
      return FALSE;
    case 'y':
      return (index == k0) ? TRUE : !isConsonant(index - 1);
    default:
      return TRUE;
  }
}

static int getMeasure() {
  int position;
  int index;
  position = 0;
  index = k0;
  while (TRUE) {
    if (index > j) {
      return position;
    }
    if (!isConsonant(index)) {
      break;
    }
    index++;
  }
  index++;
  while (TRUE) {
    while (TRUE) {
      if (index > j) {
        return position;
      }
      if (isConsonant(index)) {
        break;
      }
      index++;
    }
    index++;
    position++;

    while (TRUE) {
      if (index > j) {
        return position;
      }

      if (!isConsonant(index)) {
        break;
      }

      index++;
    }

    index++;
  }
}

/* `TRUE` when `k0, ... j` contains a vowel. */
static int
vowelInStem() {
  int index;

  index = k0 - 1;

  while (++index <= j) {
    if (!isConsonant(index)) {
      return TRUE;
    }
  }

  return FALSE;
}

/* `TRUE` when `j` and `(j-1)` are the same consonant. */
static int
isDoubleConsonant(int index) {
  if (b[index] != b[index - 1]) {
    return FALSE;
  }

  return isConsonant(index);
}

/* `TRUE` when `i - 2, i - 1, i` has the form
 * `consonant - vowel - consonant` and also if the second
 * C is not `"w"`, `"x"`, or `"y"`. this is used when
 * trying to restore an `e` at the end of a short word.
 *
 * Such as:
 *
 * `cav(e)`, `lov(e)`, `hop(e)`, `crim(e)`, but `snow`,
 * `box`, `tray`.
 */
static int
cvc(int index) {
  int character;

  if (index < k0 + 2 || !isConsonant(index) || isConsonant(index - 1) || !isConsonant(index - 2)) {
    return FALSE;
  }

  character = b[index];

  if (character == 'w' || character == 'x' || character == 'y') {
    return FALSE;
  }

  return TRUE;
}

/* `ends(s)` is `TRUE` when `k0, ...k` ends with `value`. */
static int
ends(const char *value) {
  int length = value[0];

  /* Tiny speed-up. */
  if (value[length] != b[k]) {
    return FALSE;
  }

  if (length > k - k0 + 1) {
    return FALSE;
  }

  if (memcmp(b + k - length + 1, value + 1, length) != 0) {
    return FALSE;
  }

  j = k - length;

  return TRUE;
}

/* `setTo(value)` sets `(j + 1), ...k` to the characters in
 * `value`, readjusting `k`. */
static void
setTo(const char *value) {
  int length = value[0];

  memmove(b + j + 1, value + 1, length);

  k = j + length;
}

/* Set string. */
static void
replace(const char *value) {
  if (getMeasure() > 0) {
    setTo(value);
  }
}

/* `step1ab()` gets rid of plurals, `-ed`, `-ing`.
 *
 * Such as:
 *
 *   caresses  ->  caress
 *   ponies    ->  poni
 *   ties      ->  ti
 *   caress    ->  caress
 *   cats      ->  cat
 *
 *   feed      ->  feed
 *   agreed    ->  agree
 *   disabled  ->  disable
 *
 *   matting   ->  mat
 *   mating    ->  mate
 *   meeting   ->  meet
 *   milling   ->  mill
 *   messing   ->  mess
 *
 *   meetings  ->  meet
 */
static void
step1ab() {
  int character;

  if (b[k] == 's') {
    if (ends("\04" "sses")) {
      k -= 2;
    } else if (ends("\03" "ies")) {
      setTo("\01" "i");
    } else if (b[k - 1] != 's') {
      k--;
    }
  }

  if (ends("\03" "eed")) {
    if (getMeasure() > 0) {
      k--;
    }
  } else if ((ends("\02" "ed") || ends("\03" "ing")) && vowelInStem()) {
    k = j;

    if (ends("\02" "at")) {
      setTo("\03" "ate");
    } else if (ends("\02" "bl")) {
      setTo("\03" "ble");
    } else if (ends("\02" "iz")) {
      setTo("\03" "ize");
    } else if (isDoubleConsonant(k)) {
      k--;

      character = b[k];

      if (character == 'l' || character == 's' || character == 'z') {
        k++;
      }
    } else if (getMeasure() == 1 && cvc(k)) {
      setTo("\01" "e");
    }
  }
}

/* `step1c()` turns terminal `"y"` to `"i"` when there
 * is another vowel in the stem. */
static void step1c() {
    if (ends("\01" "y") && vowelInStem()) {
        b[k] = 'i';
    }
}
/* `step2()` maps double suffices to single ones.
 * so -ization ( = -ize plus -ation) maps to -ize etc.
 * note that the string before the suffix must give
 * getMeasure() > 0. */
static void step2() {
    switch (b[k - 1]) {
        case 'a':
        if (ends("\07" "ational")) {
            replace("\03" "ate");
            break;
        }

        if (ends("\06" "tional")) {
            replace("\04" "tion");
            break;
        }

        break;
        case 'c':
        if (ends("\04" "enci")) {
            replace("\04" "ence");
            break;
        }

        if (ends("\04" "anci")) {
            replace("\04" "ance");
            break;
        }

        break;
        case 'e':
        if (ends("\04" "izer")) {
            replace("\03" "ize");
            break;
        }

        break;
        case 'l':
        /* --DEPARTURE--: To match the published algorithm,
        * replace this line with:
        *
        * ```
        * if (ends("\04" "abli")) {
        *     replace("\04" "able");
        *
        *     break;
        * }
        * ```
        */
        if (ends("\03" "bli")) {
            replace("\03" "ble");
            break;
        }

        if (ends("\04" "alli")) {
            replace("\02" "al");
            break;
        }

        if (ends("\05" "entli")) {
            replace("\03" "ent");
            break;
        }

        if (ends("\03" "eli")) {
            replace("\01" "e");
            break;
        }

        if (ends("\05" "ousli")) {
            replace("\03" "ous");
            break;
        }

        break;
        case 'o':
        if (ends("\07" "ization")) {
            replace("\03" "ize");
            break;
        }

        if (ends("\05" "ation")) {
            replace("\03" "ate");
            break;
        }

        if (ends("\04" "ator")) {
            replace("\03" "ate");
            break;
        }

        break;
        case 's':
        if (ends("\05" "alism")) {
            replace("\02" "al");
            break;
        }

        if (ends("\07" "iveness")) {
            replace("\03" "ive");
            break;
        }

        if (ends("\07" "fulness")) {
            replace("\03" "ful");
            break;
        }

        if (ends("\07" "ousness")) {
            replace("\03" "ous");
            break;
        }

        break;
        case 't':
        if (ends("\05" "aliti")) {
            replace("\02" "al");
            break;
        }

        if (ends("\05" "iviti")) {
            replace("\03" "ive");
            break;
        }

        if (ends("\06" "biliti")) {
            replace("\03" "ble");
            break;
        }

        break;
        /* --DEPARTURE--: To match the published algorithm, delete this line. */
        case 'g':
        if (ends("\04" "logi")) {
            replace("\03" "log");
            break;
        }
    }
}

/* `step3()` deals with -ic-, -full, -ness etc.
 * similar strategy to step2. */
static void step3() {
  switch (b[k]) {
    case 'e':
      if (ends("\05" "icate")) {
        replace("\02" "ic");
        break;
      }

      if (ends("\05" "ative")) {
        replace("\00" "");
        break;
      }

      if (ends("\05" "alize")) {
        replace("\02" "al");
        break;
      }

      break;
    case 'i':
      if (ends("\05" "iciti")) {
        replace("\02" "ic");
        break;
      }

      break;
    case 'l':
      if (ends("\04" "ical")) {
        replace("\02" "ic");
        break;
      }

      if (ends("\03" "ful")) {
        replace("\00" "");
        break;
      }

      break;
    case 's':
      if (ends("\04" "ness")) {
        replace("\00" "");
        break;
      }

      break;
  }
}

/* `step4()` takes off -ant, -ence etc., in
 * context <c>vcvc<v>. */
static void step4() {
    switch (b[k - 1]) {
    case 'a':
        if (ends("\02" "al")) {
        break;
        }

        return;
    case 'c':
        if (ends("\04" "ance")) {
        break;
        }

        if (ends("\04" "ence")) {
        break;
        }

        return;
    case 'e':
        if (ends("\02" "er")) {
        break;
        }

        return;
    case 'i':
        if (ends("\02" "ic")) {
        break;
        }

        return;
    case 'l':
        if (ends("\04" "able")) {
        break;
        }

        if (ends("\04" "ible")) {
        break;
        }

        return;
    case 'n':
        if (ends("\03" "ant")) {
        break;
        }

        if (ends("\05" "ement")) {
        break;
        }

        if (ends("\04" "ment")) {
        break;
        }

        if (ends("\03" "ent")) {
        break;
        }

        return;
    case 'o':
        if (ends("\03" "ion") && j >= k0 && (b[j] == 's' || b[j] == 't')) {
        break;
        }

        /* takes care of -ous */
        if (ends("\02" "ou")) {
        break;
        }

        return;
    case 's':
        if (ends("\03" "ism")) {
        break;
        }

        return;
    case 't':
        if (ends("\03" "ate")) {
        break;
        }

        if (ends("\03" "iti")) {
        break;
        }

        return;
    case 'u':
        if (ends("\03" "ous")) {
        break;
        }

        return;
    case 'v':
        if (ends("\03" "ive")) {
        break;
        }

        return;
    case 'z':
        if (ends("\03" "ize")) {
        break;
        }

        return;
    default:
        return;
    }

    if (getMeasure() > 1) {
    k = j;
    }
}

/* `step5()` removes a final `-e` if `getMeasure()` is
 * greater than `1`, and changes `-ll` to `-l` if
 * `getMeasure()` is greater than `1`. */
static void step5() {
    int a;

    j = k;

    if (b[k] == 'e') {
    a = getMeasure();

    if (a > 1 || (a == 1 && !cvc(k - 1))) {
        k--;
    }
    }

    if (b[k] == 'l' && isDoubleConsonant(k) && getMeasure() > 1) {
    k--;
    }
}

/* In `stem(p, i, j)`, `p` is a `char` pointer, and the
 * string to be stemmed is from `p[i]` to
 * `p[j]` (inclusive).
 *
 * Typically, `i` is zero and `j` is the offset to the
 * last character of a string, `(p[j + 1] == '\0')`.
 * The stemmer adjusts the characters `p[i]` ... `p[j]`
 * and returns the new end-point of the string, `k`.
 *
 * Stemming never increases word length, so `i <= k <= j`.
 *
 * To turn the stemmer into a module, declare 'stem' as
 * extern, and delete the remainder of this file. */
int stem(char *p, int index, int position) {
  /* Copy the parameters into statics. */
    b = p;
    k = position;
    k0 = index;

    if (k <= k0 + 1) {
    return k;
    }
    step1ab();

    if (k > k0) {
    step1c();
    step2();
    step3();
    step4();
    step5();
    }

    return k;
}
string new_form(string word){
    char *w = &word[0];
    int end = stem(w,0,strlen(w)- 1);
    return word.substr(0,end+1);   
}

QNA_tool::QNA_tool(){
    // Implement your function here
    vector<long long int> corresponding_count;
    vector<string> total_word;
	vector<vector<string>> content;
	vector<string> row;
	string line, word;
	fstream file ("unigram_freq.csv", ios::in);
    main_Trie = new Trie();
    corpus_Trie = new Trie();  
    // cout << "hello1" << endl;
    // main_Trie = new Trie();
	if(file.is_open()){
		while(getline(file, line)){
			row.clear();
			stringstream str(line);
			while(getline(str, word, ',')){
				row.push_back(word);
            }
			content.push_back(row);
		}
	}
    // cout << content.size() << endl;
	for(int i=1;i<content.size();i++){
        stringstream count;
        count << content[i][1];
        // long long int x = stoll(content[i][1]);
        long long int x;
        count >> x;
    //   string word = "wol";
    //   char *w = &word[0];
    //   int end = stem(w,0,strlen(w)- 1);
    //   cout << word.substr(0,end+1) << endl;
        main_Trie->insert(content[i][0],x);
        string wx = new_form(content[i][0]);
        // cout << i << ": " << wx << " " << x << endl;
        // cout << i << " " << content.size() << endl;
	}
    // corpus_Trie = new Trie();    
    // cout << "hello2" << endl;
}

QNA_tool::~QNA_tool(){
    // delete main_Trie;
    // delete corpus_Trie; 
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){

    if (((list.size() <= 0)) || (list.back()->para_num != paragraph)||(list.back()->book_code != book_code)||(list.back()->page_num != page)){
        Paragraph *p1 = new Paragraph(paragraph, book_code, page);
        list.push_back(p1);
    }
    int i = 0;
    int j = 0;
    while (i < sentence.size()){
        while (j < sentence.size())
        {
            if ((sentence.at(j) >= char('a')) && (sentence.at(j) <= char('z'))){
                j++;
            }
            else if ((sentence.at(j) == ' ') || (sentence.at(j) == '.') || (sentence.at(j) == ',') || (sentence.at(j) == '-') || (sentence.at(j) == ':') || (sentence.at(j) == '!') || (sentence.at(j) == '\"') || (sentence.at(j) == '\'') || (sentence.at(j) == '(') || (sentence.at(j) == ')') || (sentence.at(j) == '?') || (sentence.at(j) == '[') || (sentence.at(j) == ']') || (sentence.at(j) == ';') || (sentence.at(j) == '@')){
                break;
            }
            else
            {
                j++;
            }
        }
        string word = sentence.substr(i, j - i);
        list.back()->para_trie->insert(word);
        corpus_Trie->insert(word);
        j++;
        i = j;
    }
    return;
}

Node *QNA_tool::get_top_k_para(string question, int k){
    // Implement your function here
    Maxheap * M_Heap = new Maxheap();
    int i = 0;
    int j = 0;
    vector<string> words;
    vector<double> score;
    while (i < question.size()){
        while (j < question.size())
        {
            if ((question.at(j) >= char('a')) && (question.at(j) <= char('z'))){
                j++;
            }
            else if ((question.at(j) == ' ') || (question.at(j) == '.') || (question.at(j) == ',') || (question.at(j) == '-') || (question.at(j) == ':') || (question.at(j) == '!') || (question.at(j) == '\"') || (question.at(j) == '\'') || (question.at(j) == '(') || (question.at(j) == ')') || (question.at(j) == '?') || (question.at(j) == '[') || (question.at(j) == ']') || (question.at(j) == ';') || (question.at(j) == '@')){
                break;
            }
            else
            {
                j++;
            }
        }
        string word = question.substr(i, j - i);
        words.push_back(word);
        long long int den = main_Trie->getCount(word)+1;
        long long int num = corpus_Trie->getCount(word)+1;
        double d = double(num)/double(den);
        score.push_back(d);
        j++;
        i = j;
    }    
    vector<info> arr;

    for(int i=0;i<list.size();i++){
        long double para_score = 0;
        for(int j=0;j<words.size();j++){
            para_score+=(double(list[i]->para_trie->getCount(words[j]))*score[j]);
        }
        info qaz = {list[i]->book_code,list[i]->page_num,list[i]->para_num,para_score};
        arr.push_back(qaz);
    }
    M_Heap->buildheap(arr);
    Node * head=nullptr;
    info h = M_Heap->del();
    //cout << h.book << " " << h.page << " " << h.para << " " << h.score << endl;
    head = new Node(h.book,h.page,h.para,0,0);
    head->left = nullptr;
    head->right = nullptr;
    Node * recent=head;
    for(int i=1;i<k;i++){
        Node * m;
        info g = M_Heap->del();
        m = new Node(g.book,g.page,g.para,0,0);
        //cout << g.book << " " << g.page << " " << g.para << " " << g.score << endl;
        m->right = nullptr;
        m->left = recent;
        recent->right = m;
        recent = m;
    }
    return head;
}
Node *QNA_tool::get_new_top_k_para(string question, int k){
    // Implement your function here
    Maxheap * M_Heap = new Maxheap();
    int i = 0;
    int j = 0;
    vector<string> words;
    vector<double> score;
    while (i < question.size()){
        while (j < question.size())
        {
            if ((question.at(j) >= char('a')) && (question.at(j) <= char('z'))){
                j++;
            }
            else if ((question.at(j) == ' ') || (question.at(j) == '.') || (question.at(j) == ',') || (question.at(j) == '-') || (question.at(j) == ':') || (question.at(j) == '!') || (question.at(j) == '\"') || (question.at(j) == '\'') || (question.at(j) == '(') || (question.at(j) == ')') || (question.at(j) == '?') || (question.at(j) == '[') || (question.at(j) == ']') || (question.at(j) == ';') || (question.at(j) == '@')){
                break;
            }
            else
            {
                j++;
            }
        }
        string word = question.substr(i, j - i);
        bool helpful_word = true;
        for(string s:remove1){
          if(s==word){
            helpful_word = false;
            break;
          }
        }
        for(string s:remove2){
          if(s==word){
            helpful_word = false;
            break;
          }
        }
        if(helpful_word){
          words.push_back(word);
          long long int num = corpus_Trie->getCount(word)+1;
          double d = double(1)/double(num);
          score.push_back(d);
        }
        j++;
        i = j;
    }
    vector<info> arr;

    for(int i=0;i<list.size();i++){
        long double para_score = 0;
        for(int j=0;j<words.size();j++){
            para_score+=(double(list[i]->para_trie->getCount(words[j]))*score[j]);
        }

        info qaz = {list[i]->book_code,list[i]->page_num,list[i]->para_num,para_score};
        arr.push_back(qaz);
    }
    M_Heap->buildheap(arr);
    Node * head=nullptr;
    info h = M_Heap->del();
    //cout << h.book << " " << h.page << " " << h.para << " " << h.score << endl;
    head = new Node(h.book,h.page,h.para,0,0);
    head->left = nullptr;
    head->right = nullptr;
    Node * recent=head;
    for(int i=1;i<k;i++){
        Node * m;
        info g = M_Heap->del();
        m = new Node(g.book,g.page,g.para,0,0);
        //cout << g.book << " " << g.page << " " << g.para << " " << g.score << endl;
        m->right = nullptr;
        m->left = recent;
        recent->right = m;
        recent = m;
    }
    return head;
}
void QNA_tool::query(string question, string filename){
    Node * root = get_new_top_k_para(question,5);
    query_llm(filename,root,5,"YOUR_API_KEY",question);
    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
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
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }
        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }
    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question){
    Node* traverse = root;
    int num_paragraph = 0;
    cout << question << endl;
    ofstream oufile("chatgpt_response.txt");
    ofstream ofile("hugbot_response.txt");
    oufile << "Query: " << question << endl;
    oufile << endl;
    oufile << "ChatGPT Response: " ;
    ofile << "Query: " << question << endl;
    ofile << endl;
    ofile << "HuggingChat Response: ";
    oufile.close();
    ofile.close();
    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, give the answer and while giving the answer please ensure that 1. Give any answer you are able to interpret,and make sure to not give any vague answer to the above question like it is not clear,etc 2. Make sure that for a subjective question you return a more structured answer, 3. For objective type of question please ensure the correctness of answer, etc." << endl;
    outfile << "question: " << question << endl; 
    outfile.close();
    string command_chatgpt = "python3 gpt_call.py ";
    command_chatgpt += API_KEY;
    string command_hugbot = "python3 hug_call.py ";
    command_chatgpt += " ";
    for(int i=0;i<k;i++){
        string c = "paragraph_";
        c += to_string(i);
        c += ".txt ";
        command_chatgpt += c;
        command_hugbot += c;
    }
    command_chatgpt += " query.txt chatgpt_response.txt";
    command_hugbot += " query.txt hugbot_response.txt";

    system(command_chatgpt.c_str());
    cout << endl;
    system(command_hugbot.c_str());
    return;
}
void checker(Node*ans,int k){
    for(int i=0;i<k;i++){
        cout << ans->book_code << " " << ans->page << " " << ans->paragraph << " " << ans->sentence_no << " " << ans->offset << endl;
        ans = ans->right;        
    }
}
