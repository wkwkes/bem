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
            if(Terms.at(i)->getValueID() != VarID) {
                std::cout << "(";
                Terms.at(i)->Print();
                std::cout << ")";
            } else {
                Terms.at(i)->Print();
                if(i != Terms.size()-1) {
                    std::cout <<" ";
                }
            }
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
            if(Terms.at(i)->getValueID() != VarID) {
                std::cout << "(";
                Terms.at(i)->PrintD();
                std::cout << ")";
            } else {
                Terms.at(i)->PrintD();
                if(i != Terms.size()-1) {
                    std::cout <<" ";
                }
            }
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

void TermAST::shift(int d, int c) {
    if (ID == AppTermID) {
        if(Terms.size() < 2) {
            std::cout << "error in shift :too few terms\n";
            return;
        }
        Terms[0]->shift(d, c);
        Terms[1]->shift(d, c);
    } else if (ID == AbsTermID) {
        Term->shift(d, c+1);
    } else if (ID == VarID) {
        if (c<=DIndex) {
            DIndex += d;
        }
    } else {
        std::cout << "error in shift (IDが変)\n";
    }
}

void TermAST::subst(int var, TermAST *term) {//[t1 -> t2]this
    if (ID == AbsTermID) {
        term->shift(1, 0);
        Term->subst(var + 1, term);
        return;
    } else if (ID == AppTermID) {
        for (auto t : Terms) {
            t->subst(var, term);
        }
        return;
    } else if (ID == VarID) {
        if (DIndex == var) {
            ID = term->ID;
            Name = term->Name;
            DIndex = term->DIndex;
            Term = term->Term;
            Terms = term->Terms;
            return;
        } else {
            return;
        }
    } else {
        std::cout << "error in apply()\n";
        return;
    }
}

void TermAST::apply() {
    if(Terms.size() < 2) {
        std::cout << "error: too few terms\n";
        return;
    }
    if(ID != AppTermID || Terms[0]->getValueID() != AbsTermID) {
        std::cout << "error: cannot apply these terms\n";
        return;
    }
    Terms[1]->shift(1, 0);
    Terms[0]->Term->subst(0, Terms[1]);
    Terms[0]->Term->shift(-1, 0);
    Terms[0] = Terms[0]->Term;
    Terms.erase(Terms.begin()+1);
    if (Terms.size() == 1) {
        ID = Terms[0]->getValueID();
        Term = Terms[0]->Term;
        DIndex = Terms[0]->DIndex;
        Name = Terms[0]->Name;
        Terms = Terms[0]->Terms;
    }
    return;
}

