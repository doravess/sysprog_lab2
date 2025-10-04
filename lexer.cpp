#include "lexer.h"
#include <cctype>
#include <unordered_set>

using namespace std;

const unordered_set<string> reserved = {
        "int","float","char","if","else","for","while","return","const","void"
};

bool isIdentStart(char c) { return isalpha((unsigned char)c) || c=='_'; }
bool isIdentPart(char c)  { return isalnum((unsigned char)c) || c=='_'; }

vector<Token> lex(const string &src) {
    vector<Token> tokens;
    int line = 1, col = 1;
    size_t i = 0;

    auto get = [&]() { char c = src[i++]; if(c=='\n'){line++; col=1;} else col++; return c; };
    auto peek = [&](size_t ahead=0){ return (i+ahead<src.size()? src[i+ahead]:'\0'); };

    while(i<src.size()) {
        char c = peek();
        if(isspace(c)) { get(); continue; }

        int tokLine = line, tokCol = col;

        // Коментарі //
        if(c=='/' && peek(1)=='/') {
            string s; s+=get(); s+=get();
            while(peek()!='\n' && i<src.size()) s+=get();
            tokens.push_back({Comment,s,tokLine,tokCol});
            continue;
        }

        // Директива препроцесора
        if(c=='#') {
            string s; while(i<src.size() && peek()!='\n') s+=get();
            tokens.push_back({Preprocessor,s,tokLine,tokCol});
            continue;
        }

        // Рядкові та символьні літерали
        if(c=='"') { string s; s+=get(); while(i<src.size() && peek()!='"') s+=get(); if(i<src.size()) s+=get(); tokens.push_back({StringLiteral,s,tokLine,tokCol}); continue; }
        if(c=='\''){ string s; s+=get(); while(i<src.size() && peek()!='\'') s+=get(); if(i<src.size()) s+=get(); tokens.push_back({CharLiteral,s,tokLine,tokCol}); continue; }

        // Числа
        if(isdigit((unsigned char)c)){
            string s; while(isdigit((unsigned char)peek())) s+=get();
            tokens.push_back({Number,s,tokLine,tokCol});
            continue;
        }

        // Ідентифікатори та ключові слова
        if(isIdentStart(c)){
            string s; s+=get(); while(isIdentPart(peek())) s+=get();
            TokenType t = reserved.count(s) ? Reserved : Identifier;
            tokens.push_back({t,s,tokLine,tokCol});
            continue;
        }

        // Оператори та розділові знаки (спрощено)
        string s; s+=get(); tokens.push_back({Operator,s,tokLine,tokCol});
    }

    return tokens;
}

string tokenTypeName(TokenType t) {
    switch(t){
        case Identifier: return "Identifier";
        case Reserved: return "Reserved";
        case Number: return "Number";
        case StringLiteral: return "StringLiteral";
        case CharLiteral: return "CharLiteral";
        case Preprocessor: return "Preprocessor";
        case Comment: return "Comment";
        case Operator: return "Operator";
        case Punctuator: return "Punctuator";
        case Unknown: return "Unknown";
    }
    return "Token";
}
