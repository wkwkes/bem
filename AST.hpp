#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
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
    protected:
        AstID ID;

    public:
        BaseAST(AstID id) : ID(id) {}
        virtual ~BaseAST() {}
        AstID getValueID() const {
            return ID;
        }
};

class ToplevelAST : public BaseAST{
    private:
        //std::vector<TermAST*> Terms;
        TermAST *Term;

    public:
        ToplevelAST(TermAST *term) : Term(term), BaseAST(ToplevelID) {}
        ~ToplevelAST();
        //bool empty();
        TermAST *getTerm() {
            return Term;
        }
        void Print();
};

class TermAST : public BaseAST{
    private:
        std::vector<TermAST*> Terms;
        TermAST *Term;
        std::string Name;
        //int DIndex;

    public:
        TermAST(std::vector<TermAST*> &terms) 
            : BaseAST(AppTermID), Terms(terms) {}
        TermAST(const std::string name, TermAST *term) 
            : BaseAST(AbsTermID), Name(name), Term(term) {}
        TermAST(const std::string name) : BaseAST(VarID), Name(name) {}
        ~TermAST();
        BaseAST *getTerm() {
            if(ID == AbsTermID) {
                return Term;
            } else {
                std::cout << "error in Term's Type\n";
                return NULL;
            }
        }
        BaseAST *getTerm(int i) {
            if(i>=Terms.size() || ID != AppTermID) {
                std::cout << "error in Term's Type\n";
                return NULL;
            } else {
                return Terms.at(i);
            }
        }
        std::string getVar() {
            return Name;
        }
        void Print();/*
        void setDIndex(int n) {
            DIndex = n;
        }
        int getDIndex() {
            return DIndex;
        }*/
};

/*
class VarAST : public BaseAST {
    private:
        std::string Var;
    public:
        VarAST(const std::string var) : AstID(VarID), Var(var) {}
        ~VarAST(){};
        std::string getVar() {
            return Var;
        }
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
