#include "parser.hpp"

Parser::Parser(std::string filename) {
    Tokens = LexicalAnalysis(filename);
    Ctx = Tokens->getCtx();
    Tokens->printTokens();
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
    //とりあえず一つのラムダ項のみ許す感じでいくが最終的にはwhileで処理する
    TA = new ToplevelAST(visitTerm(), Ctx);
    if(TA) {
        return true;
    }
    return false;
}

TermAST *Parser::visitTerm() {
    if(Tokens->getCurType() == TOK_LAM) {
        //std::cout <<"TOK_LAM:" <<Tokens->getCurString() << std::endl;
        if(!Tokens->nextToken()) {//var
            std::cout << "error in lexer (visitTerm1)\n";
            return NULL;
        }
        //std::cout <<"var    :" <<Tokens->getCurString() << std::endl;
        std::string var = Tokens->getCurString();
        if(!Tokens->nextToken()) {//DOT
            std::cout << "error in lexer (visitTerm2)\n";
            return NULL;
        }
        //std::cout <<"dot    :" <<Tokens->getCurString() << std::endl;
        if(!Tokens->nextToken()) {//term
            std::cout << "error in lexer (visitTerm1)\n";
            return NULL;
        }
        //std::cout <<"term   :\n(";
        TermAST *term = visitTerm();
        //std::cout <<")\n";
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


int main(int argc, char **argv) {
    Parser *parser = new Parser(argv[1]);
    std::cout << "start parsing \n";
    if(parser->doParse()) {
        std::cout << "success";
        std::cout << "\nCtx\n"; 
        parser->getToplevelAST()->toDeBrujin();
        parser->Print();
        std::cout<<"\n\n";
        parser->PrintCtx();
        parser->PrintD();
        std::cout << std::endl;
    } else {
        std::cout << "error exists\n";
        parser->Print();
        std::cout << std::endl;
    }
}




