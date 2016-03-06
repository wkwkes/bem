#ifndef HEV_HPP
#define HEV_HPP

#include <iostream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "APP.hpp"
#include "AST.hpp"
#include "parser.hpp"

typedef class Eval {
    private:
        ToplevelAST *TA;

    public:
        Eval(ToplevelAST *ta) : TA(ta) {}
        ~Eval() {}
        void hbeta();
        bool isEvalable();
} Eval;

#endif
