#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <map>
#include <vector>
#include "APP.hpp"

class BaseAST;
class ToplevelAST;
class TermAST;
class VarAST;

enum AstID {
    ToplevelID,
    AppTermID,
    AbsTermID,
    VarID
};

class BaseAST {
    private:
        AstID ID;

    public:
        BaseAST(AstID id) : ID(id) {}
        AstID getValueID() const {
            return ID;
        }
};

class ToplevelAST {
    private:
        //std::vector<TermAST*> Terms;
        TermAST *Term;

    public:
        ToplevelAST() {}
        ~ToplevelAST();
        bool empty();
        TermAST *getTerm() {
            return Term;
        }
};

class TermAST : public BaseAST{
    private:
        //std::string TType;
        vector<BaseAST*> Terms;
        VarAST *Var;
        TermAST *Term;

    public:
        TermAST(vector<BaseAST*> &terms) 
            : BaseAST(AppTermID), Terms(terms) {}
        TermAST(VarAST *var, TermAST term) 
            : BaseAST(AbsTermID), Var(var), Term(term) {}
        ~AppTermAST();
        BaseAST *getTerm() {
            if(ID == AbsTermID) {
                return Term;
            } else {
                std::cout << "error in Term's Type\n";
                return NULL;
            }
        BaseAST *getTerm(int i) {
            if(i>=Terms.size() || ID != AppTermID) {
                std::cout << "error in Term's Type\n";
                return NULL;
            } else {
                return Terms.at(i);
            }
        }
};

class VarAST : public BaseAST {
    private:
        std::string Var;
    public:
        VarAST(const std::string var) : AstID(VarID), Var(var) {}
        ~VarAST(){};
        std::string getVar() {
            return Var;
        }
/*
class AppTermAST {
    private:
        vector<BaseAST*> Terms;
    public:
        AppTermAST(vector<BaseAST*> &terms) : Terms(terms) {}
        ~AppTermAST();
        BaseAST getTerm(int i) {
            if(i>=Terms.size()) {
                return NULL;
            } else {
                return Terms.at(i);
            }
        }
};

class AbsTermAST {
    private:
        VarAST *Var;
        TermAST
    public:
*/



#endif
