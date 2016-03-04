#include "AST.hpp"

ToplevelAST::~ToplevelAST() {
    SAFE_DELETE(Term);
}

void ToplevelAST::Print() {
    Term->Print();
}

void ToplevelAST::PrintD() {
    Term->PrintD();
}

void ToplevelAST::toDeBrujin() {
    //std::cout << "start ToplevelAST::toDeBrujin()\n";
    Term->toDeBrujin(Ctx, *new std::vector<std::string>());
    //std::cout << "end ToplevelAST::toDeBrujin()\n";
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
        std::cout << "{"<<Name<<", "<<DIndex<<"}" ;
    } else {
        std::cout << "some error in TermAST::print()" << std::endl;
    }
}

void TermAST::PrintD() {
    if (ID == AbsTermID) {
        std::cout << "λ"<<".";
        Term->PrintD();
    } else if (ID == AppTermID) {
        for (int i = 0; i < Terms.size(); i++) {
            std::cout << "(";
            Terms.at(i)->PrintD();
            std::cout << ")";
        }
    } else if (ID == VarID) {
        std::cout <<DIndex;
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

void TermAST::toDeBrujin(std::map<std::string, int> &ctx, std::vector<std::string> env) {
    //std::cout<<"start TermAST::toDeBrujin()\n";
    if(ID == AppTermID) {
    //std::cout<<"start AppID\n";
        for (auto &term : Terms) {
            term->toDeBrujin(ctx, env);
        }
    //std::cout<<"end AppID\n";
    } else if (ID == AbsTermID) {
    //std::cout<<"start AbsTermID\n";
        env.push_back(Name);
        /*std::cout << "print env at lambda " << Name<<std::endl;
        for (auto itr : env) {
            std::cout << itr <<", ";
        }
        std::cout << "\n";*/
        Term->toDeBrujin(ctx, env);
    //std::cout<<"end AbsTermID\n";
    } else if (ID == VarID) {
    //std::cout<<"start VarID\n";
        //std::cout << "print env at"<< Name<<": ";
        for (int i = env.size()-1; i >= 0; i--) {
            //std::cout << env[i] <<"("<<i<<")"<<" ";
            if (env[i] == Name) {
                DIndex = env.size() - i - 1;
                break;
            } 
        }
            //std::cout << "\n index of "<<Name <<":"<<DIndex<<std::endl;
        if(DIndex < 0) {
            DIndex = ctx[Name] + env.size();
        }
            //std::cout << "index of "<<Name <<":"<<DIndex<<std::endl;
    //std::cout<<"end VarID\n";
    } else {
        std::cout << "error in toDeBrujin()\n";
    }
}



