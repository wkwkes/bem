#include "parser.hpp"

Parser::Parser(std::string filename) {
    Tokens = LexicalAnalysis(filename);
    Ctx = Tokens->getCtx();
}

bool Parser::doParse() {
    if(!Tokens) {
        fprintf(stderr, "error at lexer (doParse)\n");
        return false;
    } else {
        return visitToplevel();
    }
}

ToplevelAST &Parser::getAST() {
    if(TA) {
        return *TA;
    } else {
        return *(new ToplevelAST(NULL));
    }
}

bool Parser::visitToplevel() {
    TA = new ToplevelAST(visitTerm(), Ctx);
    if(TA) {
        return true;
    }
    return false;
}

TermAST *Parser::visitTerm() {
    if(Tokens->getCurType() == TOK_LAM) {
        if(!Tokens->nextToken()) {//var
            std::cout << "error in lexer (visitTerm1)\n";
            return NULL;
        }
        std::string var = Tokens->getCurString();
        if(!Tokens->nextToken()) {//DOT
            std::cout << "error in lexer (visitTerm2)\n";
            return NULL;
        }
        if(!Tokens->nextToken()) {//term
            std::cout << "error in lexer (visitTerm1)\n";
            return NULL;
        }
        TermAST *term = visitTerm();
        return new TermAST(var, term);
    } else {
        std::vector<TermAST*> terms(0);
        while(Tokens->getCurType() == TOK_ID || 
                Tokens->getCurType() == TOK_LPA) {
            if (Tokens->getCurType() == TOK_LPA) {
                if(!Tokens->nextToken()) {
                    std::cout << "error in lexer (visitTerm3)\n";
                    return NULL;
                }
                terms.push_back(visitTerm());
                if(Tokens->getCurType() != TOK_RPA) {
                    std::cout << "error in AppTerm\n";
                    return NULL;
                }
                if(!Tokens->nextToken()) {
                    std::cout << "error in lexer (visitTerm4)\n";
                    return NULL;
                }
            } else if (Tokens->getCurType() == TOK_ID) {
                terms.push_back(new TermAST(Tokens->getCurString()));
                if(!Tokens->nextToken()) {
                    std::cout << "error in lexer (visitTerm5)\n";
                    return NULL;
                }
            }
        }
        return new TermAST(terms);
    }
}
