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
        std::map<std::string, int> Ctx;

    public:
        ToplevelAST(TermAST *term, std::map<std::string, int> &ctx) : Term(term), BaseAST(ToplevelID), Ctx(ctx) {}
        ToplevelAST(TermAST *term) : Term(term), BaseAST(ToplevelID) {}
        ~ToplevelAST();
        //bool empty();
        TermAST *getTerm() {
            return Term;
        }
        void Print();
        void PrintCtx() {
            for(auto itr : Ctx) {
                std::cout << "key : "<< itr.first << ", val : " << itr.second<<std::endl;
            }
        }
        void toDeBrujin();
        void PrintD();
};

class TermAST : public BaseAST{
    private:
        std::vector<TermAST*> Terms;
        TermAST *Term;
        std::string Name;
        int DIndex;

    public:
        TermAST(std::vector<TermAST*> &terms) 
            : BaseAST(AppTermID), Terms(terms), DIndex(-1) {}
        TermAST(const std::string name, TermAST *term) 
            : BaseAST(AbsTermID), Name(name), Term(term), DIndex(-1) {}
        TermAST(const std::string name) : BaseAST(VarID), Name(name), DIndex(-1) {}
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
        void Print();
        void setDIndex(int n) {
            DIndex = n;
        }
        int getDIndex() {
            return DIndex;
        }
        void toDeBrujin(std::map<std::string, int> &ctx, std::vector<std::string> env);
        void PrintD();
        void apply();
        void shift();

};

#endif
