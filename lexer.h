#pragma once
#include <string>
#include <vector>

using namespace std;

enum TokenType {
    Identifier, Reserved, Number, StringLiteral, CharLiteral,
    Preprocessor, Comment, Operator, Punctuator, Unknown
};

struct Token {
    TokenType type;
    string text;
    int line;
    int col;
};

vector<Token> lex(const string &source);
string tokenTypeName(TokenType t);
