#include "AST.hpp"

ToplevelAST::~ToplevelAST() {
    SAFE_DELETE(Term);
}

TermAST::~TermAST() {
    if (ID == AppTermID) {
        for (int i = 0; i < Terms.size(); i++) {
            SAFE_DELETE(Terms[i]);
        }
        Terms.clear();
    }
    if (ID == AbsTermID) {
        SAFE_DELETE(Var);
        SAFE_DELETE(Term);
    }
}


