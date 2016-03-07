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
        void setID(AstID id) {
            ID = id;
        }
};

class ToplevelAST : public BaseAST{
    private:
        //std::vector<TermAST*> Terms;
        TermAST *Term;
        std::map<std::string, int> Ctx;

    public:
        void PrintDD();
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
        void fromDeBrujin();
        void PrintD();
        std::map<std::string, int> getCtx() {
            return Ctx;
        }
        void Gen();
        void heval();
};

class TermAST : public BaseAST {
    private:
        std::map<std::string, int> Ctx;
        std::vector<TermAST*> Terms;
        TermAST *Term;
        std::string Name;
        int DIndex;

    public:
        TermAST(std::vector<TermAST*> terms) 
            : BaseAST(AppTermID), Term(NULL), DIndex(-1) {
                Terms.clear();
                for (auto itr: terms) {
                    Terms.push_back(new TermAST(*itr));
                }
            }
        TermAST(const std::string name, TermAST *term) 
            : BaseAST(AbsTermID), Name(name), Term(new TermAST(*term)), DIndex(-1) {}
        TermAST(const std::string name) : BaseAST(VarID), Name(name), DIndex(-1), Term(NULL) {}
        TermAST(const TermAST&);
        TermAST& operator=(const TermAST&);
        ~TermAST();
        TermAST *getTerm() {
            if(ID == AbsTermID) {
                return Term;
            } else {
                std::cout << "error in Term's Type\n";
                return NULL;
            }
        }
        TermAST *getTerm(int i) {
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
        //void Print();
        void PrintDD();
        void setDIndex(int n) {
            DIndex = n;
        }
        int getDIndex() {
            return DIndex;
        }
        void setCtx(std::map<std::string, int> &ctx){
            Ctx = ctx;
        }
        void toDeBrujin(std::map<std::string, int> &ctx, std::vector<std::string> env);
        //void PrintD();
        void subst(int var, TermAST *term);
        void shift(int d, int c);
        void apply();
        void Printtm(std::map<std::string, int> &ctx, std::vector<std::string> env);
        void PrinttmD(std::map<std::string, int> &ctx, std::vector<std::string> env);
        std::string pickfresh(std::map<std::string, int> &ctx, std::vector<std::string> env, std::string str);
        void Gen(std::map<std::string, int> &ctx, std::vector<std::string> env, bool top);
        bool isEvalable();
        int getTermssize() {
            return Terms.size();
        }
        void hbeta();
};

#endif
