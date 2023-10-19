//#include <map>
//#include <string> 
//#include <iostream>
//Tokenizer of the compilier
using namespace std; 

enum KeyToken{
    IDENT, ERR, IN, AND, OR, TRU, FALS, NOT, IMPORT, //Done
    EQUALCMP, EQUALS, NOTEQUAL, GREATER, GREATEREQUALS, LESS, LESSEQUALS, INSERTION, REMOVAL, INCREMENT, DECREMENT, //Done
    OPENCURL, CLOSECURL, OPENSQUARE, CLOSESQUARE, LPAREN, RPAREN, COMMA, //Done
    PLUS, MINUS, MULT, DIV, LEFTOVER, POWERTO, POINTERTO, REFRENCETO, AS, //Done
    IF, ELSE, FOR, DO, WHILE, CASE, SWITCH, BREAK, CONTINUE, DEFAULT, RET, PRINT, PRINTLN,//Done
    OBJECT, CONSTRUCTOR, DESTRUCTOR, SEMICOL, COLON, PERIOD,//Done
    CONST, SINGULAR, VOLATILE, PUBLIC, PRIVATE, PROTECTED, //Done
    SCONST, NUMCONST, //Done
    INT, SHORT, LONG, POINTER, FLOAT, DOUBLE, STRING, BOOL, CHAR, BYTE, AUTO //Done 
};

map<string, KeyToken> keywords = {
    {"", IDENT}, {"IN", IN}, {"AND",AND}, {"OR",OR}, {"TRUE", TRU}, {"FALSE", FALS}, {"NOT", NOT},{"IMPORT", IMPORT},
    {"IF", IF}, {"ELSE", ELSE}, {"FOR", FOR}, {"DO", DO},{"WHILE", WHILE}, {"CASE", CASE}, {"SWITCH", SWITCH}, {"BREAK", BREAK}, {"CONTINUE", CONTINUE}, {"DEFAULT", DEFAULT}, {"RETURN", RET}, {"PRINT",PRINT}, {"PRINTLN", PRINTLN},
    {"OBJECT", OBJECT}, {"CONSTRUCTOR", CONSTRUCTOR}, {"DESTRUCTOR", DESTRUCTOR}, {"AS", AS},
    {"CONST", CONST}, {"SINGULAR", SINGULAR}, {"VOLATILE", VOLATILE}, {"PUBLIC", PUBLIC}, {"PRIVATE", PRIVATE}, {"PROTECTED", PROTECTED},
    {"INT", INT}, {"SHORT", SHORT}, {"LONG", LONG}, {"POINTER", POINTER}, {"FLOAT", FLOAT}, {"DOUBLE", DOUBLE}, {"STRING", STRING}, {"BOOL", BOOL}, {"CHAR", CHAR}, {"BYTE", BYTE}, {"AUTO", AUTO}
};

string keytokens[] {
    "IDENT","ERR","IN","AND","OR","TRU","FALS", "NOT", "IMPORT",
    "EQUALCMP","EQUALS", "NOTEQUAL", "GREATER", "GREATEREQUALS", "LESS", "LESSEQUALS", "INSERTION","REMOVAL", "INCREMENT", "DECREMENT",
    "OPENCURL","CLOSECURL", "OPENSQUARE", "CLOSESQUARE", "LPAREN","RPAREN", "COMMA",
    "PLUS","MINUS","MULT","DIV","LEFTOVER","POWERTO","POINTERTO","REFRENCETO", "AS",
    "IF","ELSE","FOR","DO","WHILE","CASE","SWITCH", "BREAK", "CONTINUE", "DEFAULT", "RET", "PRINT", "PRINTLN",
    "OBJECT","CONSTRUCTOR","DESTRUCTOR","SEMICOL", "COLON", "PERIOD",
    "CONST","SINGULAR", "VOLATILE", "PUBLIC","PRIVATE","PROTECTED",
    "SCONST", "NUMCONST",
    "INT","SHORT","LONG","POINTER","FLOAT","DOUBLE","STRING","BOOL","CHAR","BYTE","AUTO"
}; 
enum LexState{START, INNUM, INSTRING, INCOMMENT, INLINECOMMENT, INIDENT};

class Token{
    public:
    KeyToken token = ERR;
    string lex; 
    int ln; 
    Token(){
        token = ERR; 
        ln = -1; 
    }
    Token(KeyToken k, string lexeme, int line){
        token = k; 
        lex = lexeme; 
        line = line; 
    }

    string toString(){
        string st; 
        st+=keytokens[token]; 
        st+=" '";
        st+=lex; 
        st+="' Line:";
        st+=to_string(ln); 
        return st;
    }

    friend ostream& operator<<(ostream& s, Token t){
        string q; 
        q+= keytokens[t.token]; 
        if(t.token == IDENT) q+="("+t.lex+")";
        cout <<std::setw(10)<<q;
        cout << "\tline: " << t.ln <<endl;
        return s; 
    }
    bool operator==(Token t){
        return (token == t.token); 
    }
    bool operator!=(Token t){
        return (token != t.token); 
    }
    bool operator==(KeyToken t){
        return (token == t); 
    }
    bool operator!=(KeyToken t){
        return (token != t); 
    }
};

bool iequals(const string& a, const string& b){
    unsigned int sz = a.size();
    if (b.size() != sz) return false;
    for (unsigned int i = 0; i < sz; ++i)
        if (tolower(a[i]) != tolower(b[i]))
            return false;
    return true;
}
string toUpper(string s){
    for(int i = 0; i < s.size(); i++){
        if(isalpha(s[i]) && !isupper(s[i])) s[i] = toupper(s[i]); 
    }
    return s; 
}
bool isValidInt(char ch){
    return (isdigit(ch) || ch == '.');
}
bool isValidIdent(char ch){
    return (isalnum(ch) || ch == '_') && ch != ';' && ch != '.';
}
/**
 * @brief Get the Next Token object in the stringstream provided. Also takes a 'line' variable to keep track of possible errors.
 * The line Variable automatically increments as the tokenizer reads off of the StringStream. 
 * 
 * @param s - The refrence to the stringtream to read off of. 
 * @param line - The refrence to the line counter. Represents the current line we're on. This automatically increments as it goes along, no need to track it manually.
 * @return Token - The next token in the stream, separated by whitespace. 
 */
Token getNextToken(istream & s, int & line){
    LexState state = START;
    char ch = 0; 
    string lexeme; 
    while(s.get(ch)){
        char nextchar = s.peek(); 
        if(ch == '\n') line++; 
        switch(state){
            case START:{
            bool isswapped = false; 
            if(isspace(ch)){
                continue;  
                }
            if(ch == '@') return Token(REFRENCETO, "@", line); 
            else if(ch == ';') return Token(SEMICOL, ";", line); 
            else if (ch == '{') return Token(OPENCURL, "{", line); 
            else if (ch == '}') return Token(CLOSECURL, "}", line); 
            else if (ch == '[') return Token(OPENSQUARE, "[", line); 
            else if (ch == ']') return Token(CLOSESQUARE, "]", line); 
            else if (ch == ')') return Token(RPAREN, ")", line); 
            else if (ch == '(') return Token(LPAREN, "(", line); 
            else if (ch == '*') return Token(MULT, "*", line); 
            else if (ch == '.') return Token(PERIOD, ".", line); 
            else if (ch == '^') return Token(POWERTO, "^", line); 
            else if (ch == '%') return Token(LEFTOVER, "%", line); 
            else if (ch == ',') return Token(COMMA, ",", line); 
            else if (ch == ':') return Token(COLON, ":", line); 
            else if (ch == '!') {
                if(nextchar == '=') {s.get(); return Token(NOTEQUAL, "!=", line); }
                return Token(NOT, "!", line);
            }else if (ch == '+' && nextchar == '+') {
                s.get(); 
                return Token(INCREMENT, "++", line); }
            else if (ch == '+') return Token(PLUS, "+", line); 
            //Removal operator vs. Less Than
            else if (ch == '<' && nextchar == '<') {
                s.get();
                return Token(INSERTION, "<<", line);
            }else if(ch == '<' && nextchar == '='){
                s.get();
                return Token(LESSEQUALS, "<=", line); 
            }else if (ch == '<') return Token(LESS, "<", line); 
            //Insertion operator vs. Greater than; 
            else if(ch == '>' && nextchar == '>'){
                s.get(); 
                return Token(REMOVAL, ">>", line); 
            }else if(ch == '>' && nextchar == '='){
                s.get();
                return Token(GREATEREQUALS, ">=", line); 
            }else if (ch == '>') return Token(GREATER, ">", line);
            //Inline comment vs. Division
            else if (ch == '/' && nextchar == '/') {
                state = INLINECOMMENT; 
                continue; 
            }else if (ch == '/') return Token(DIV, "/", line); 
            //PointerTo vs Minus
            else if (ch == '-' && nextchar == '>') {
                s.get();
                return Token(POINTERTO, "->", line); 
            }else if (ch == '-' && nextchar == '-') {
                s.get();
                return Token(DECREMENT, "--", line); 
            }else if (ch == '-') return Token(MINUS, "-", line); 
            //Equals compare vs Equals assign; 
            else if (ch == '=' && nextchar == '=') {
                s.get(); 
                return Token(EQUALCMP, "==", line);
            }else if (ch == '=')  return Token(EQUALS, "=", line);  
            //multi line and/or documentation comment
            else if (ch == '?' && nextchar == '?') {
                state = INCOMMENT; 
                continue;
            }else if (isalpha(ch)) {
                if(isValidIdent(nextchar)){
                state = INIDENT; 
                }else {
                    lexeme = ch; 
                    return Token(IDENT, lexeme, line); }
                }
            else if (isdigit(ch)) {
                state = INNUM; 
                lexeme = ch; 
                if(!isValidInt(nextchar)) return Token(NUMCONST, lexeme, line); 
                continue;
            }else if (ch == '"' || ch == '\'') {
                state = INSTRING; 
                lexeme="";
                continue;
            }
            if(!isswapped)lexeme+=ch;
            break;
            }
            case INIDENT:{
            bool isvalidchar = isValidIdent(ch);// && isValidIdent(nextchar);
            bool isnextvalid = isValidIdent(nextchar);
            if(isvalidchar && !isspace(ch))lexeme+=ch; 
            if(!isnextvalid) return Token(keywords[toUpper(lexeme)], lexeme, line); 
            break;
            }
            case INNUM:{
            if(isValidInt(ch))lexeme+=ch; 
            if(!isValidInt(nextchar)) return Token(NUMCONST, lexeme, line); 
            break;
            }
            case INSTRING:
            {if(ch == '"' || ch == '\''){
                return Token(SCONST, lexeme, line);
            }
            lexeme+=ch; 
            break;
            }
            case INCOMMENT:
            if(nextchar == '?' && ch == '?') {
                s.get(ch); 
                state = START; 
            }
            break; 
            case INLINECOMMENT:
            if(ch == '\n') state = START; 
        }
    }
    return Token(ERR, string("ERROR"), -1);
}