#ifndef PARSER_HPP
#define PARSER_HPP

#include <algorithm>
#include <cstdio>
#include <cstdlib>

#include <map>
#include <string>
#include <utility>
#include <vector>
#include "APP.hpp"
#include "AST.hpp"
#include "lexer.hpp"

typedef class Parser {
    private:
        TokenStream *Tokens;
        ToplevelAST *TA;
        //std::vector<std::string> VariableTable;
        //std::map<std::string, int> Proto

        bool visitToplevel();
        //bool visitCommand();
        TermAST *visitTerm();
        std::map<std::string, int> Ctx;
    public:
        Parser(std::string filename);
        ~Parser() {
            SAFE_DELETE(TA);
            SAFE_DELETE(Tokens);
        }
        bool doParse();
        void PrintDD(){
            TA->PrintDD();
        }
        ToplevelAST &getAST();
        void Print() {
            TA->Print();
        }/*
        void setCtx(std::map<std::string, int> &ctx) {
            Ctx = ctx;
        }*/
        std::map<std::string, int> &getCtx() {
            return Ctx;
        }
        void PrintCtx() {
            TA->PrintCtx();
        }
        void PrintD() {
            TA->PrintD();
        }
        ToplevelAST *getToplevelAST() {
            return TA;
        }
        void toDeBrujin() {
            TA->toDeBrujin();
        }
} Parser;

#endif


