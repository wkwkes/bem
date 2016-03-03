#include "AST.hpp"

ToplevelAST::~ToplevelAST() {
    SAFE_DELETE(Term);
}

void ToplevelAST::Print() {
    Term->Print();
}

void TermAST::Print() {
    if (ID == AbsTermID) {
        std::cout << "lambda "<<Name<<" . ";
        Term->Print();
    } else if (ID == AppTermID) {
        for (int i = 0; i < Terms.size(); i++) {
            std::cout << "(";
            Terms.at(i)->Print();
            std::cout << ")";
        }
    } else if (ID == VarID) {
        std::cout << Name ;
    } else {
        std::cout << "some error in TermAST::print()" << std::endl;
    }
}

TermAST::~TermAST() {
    if (ID == AppTermID) {
        for (int i = 0; i < Terms.size(); i++) {
            SAFE_DELETE(Terms[i]);
        }
        Terms.clear();
    }
    if (ID == AbsTermID) {
        SAFE_DELETE(Term);
    }
}


