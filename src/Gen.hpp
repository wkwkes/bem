#ifndef GEN_HPP
#define GEN_HPP

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
#include "parser.cpp"

typedef class Gen {
    private:
        ToplevelAST *TA;

    public:
        Gen(ToplevelAST *ta) : TA(ta){}
        void doGen() {};

} Gen;

#endif
