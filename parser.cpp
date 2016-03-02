#include "parser.hpp"

Parser::Parser(std::string filename) {
    Tokens = LexicalAnalysis(filename);
}

bool Parser::doParser() {
    if(!Tokens) {
        fprintf(stderr, "error at lexer \n");
        return false;
    } else {
        return visitToplevel();
    }
}

ToplevelAST &Parser::getAST() {
    if(TU) {
        return *TA;
    } else {
        return *(new ToplevelAST());
    }
}

bool Parser::visitToplevel() {
    TA = new ToplevelAST();
    //とりあえず一つのラムダ項のみ許す感じでいくが最終的にはwhileで処理する
    return visitTerm(TA);
}
