#include "lexer.h"
#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char **argv){
    string src = R"(
#include <stdio.h>

int main() {
    int x = 42;
    printf("Hello!\n");
    return 0;  //comment
}
)";


    auto tokens = lex(src);
    for(auto &t: tokens){
        cout<<"["<<t.line<<":"<<t.col<<"]\t<"<<t.text<<" , "<<tokenTypeName(t.type)<<">\n";
    }
}
