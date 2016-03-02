#ifndef PARSER_HPP
#define PARSER_HPP

#include <algorithm>
#include <cstdio>
#include <cstdliba>

#include<map>
#include<string>
#include<vector>
//#include"APP.hpp"
//#include"AST.hpp"
#include"lexer.hpp"

typedef calss Parser {
    private:
        TokenStream * Tokens;
        TranslationUnitAST *TU;

        //std::vector<std::string> VariableTable;
        //std::map<std::string, int> Proto

        bool visitToplevel();
        //bool visitCommand();
        TermAST *visitTerm();
        AppTermAST *visitAppTerm();
        ATermAST *visitATerm();

    public:
        Parser(std::string filename);
        ~Parser() {
            SAFE_DELETE(TU);
            SAFE_DELETE(Tokens);
        }
        bool doParse();
        ToplevelAST &getAST();
} Parser;

#endif


