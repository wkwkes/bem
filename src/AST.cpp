#include "AST.hpp"

ToplevelAST::~ToplevelAST() {
    SAFE_DELETE(Term);
}

void ToplevelAST::Print() {
    Term->Printtm(Ctx, *new std::vector<std::string>());
}

void ToplevelAST::PrintD() {
    Term->PrinttmD(Ctx, *new std::vector<std::string>());
}

void ToplevelAST::toDeBrujin() {
    Term->toDeBrujin(Ctx, *new std::vector<std::string>());
}


/*void TermAST::Print() {
    Printtm(*new vector<std::string>());
}*/

std::string TermAST::pickfresh(std::vector<std::string> env, std::string str) {
    bool ch = false;
    while (!ch) {
        ch = true;
        for(auto itr : env) {
            if (itr == str) {
                str += "'";
                ch = false;
                break;
            }
        }
    }
    return str;
}

void TermAST::Printtm(std::map<std::string, int> &ctx, std::vector<std::string> env) {
    if (ID == AbsTermID) {
        env.push_back(pickfresh(env, Name));
        std::cout << "lambda "<<env[env.size() - 1]<<" . ";
        Term->Printtm(ctx, env);
    } else if (ID == AppTermID) {
        for (int i = 0; i < Terms.size(); i++) {
            if(Terms.at(i)->getValueID() != VarID) {
                std::cout << "(";
                Terms.at(i)->Printtm(ctx, env);
                std::cout << ")";
            } else {
                Terms.at(i)->Printtm(ctx, env);
                if(i != Terms.size()-1) {
                    std::cout <<" ";
                }
            }
        }
    } else if (ID == VarID) {
        if (DIndex < env.size()) {
            std::cout << "{"<<env[env.size()-1-DIndex]<<", "<<DIndex<<"}";
        } else {
            int index = DIndex - env.size();
            for (auto itr : ctx) {
                if (itr.second == index) {
                    std::cout <<"{"<< itr.first<<", "<<DIndex << "}";
                }
            }
        }
    } else {
        std::cout << "some error in TermAST::print()" << std::endl;
    }
}

void TermAST::PrinttmD(std::map<std::string, int> &ctx, std::vector<std::string> env) {
    if (ID == AbsTermID) {
        env.push_back(pickfresh(env, Name));
        std::cout << "\\"<<env[env.size() - 1]<<". ";
        Term->PrinttmD(ctx, env);
    } else if (ID == AppTermID) {
        for (int i = 0; i < Terms.size(); i++) {
            if(Terms.at(i)->getValueID() != VarID) {
                std::cout << "(";
                Terms.at(i)->PrinttmD(ctx, env);
                std::cout << ")";
            } else {
                Terms.at(i)->PrinttmD(ctx, env);
                if(i != Terms.size()-1) {
                    std::cout <<" ";
                }
            }
        }
    } else if (ID == VarID) {
        if (DIndex < env.size()) {
            std::cout << env[env.size()-1-DIndex];
        } else {
            int index = DIndex - env.size();
            for (auto itr : ctx) {
                if (itr.second == index) {
                    std::cout << itr.first;
                }
            }
        }
    } else {
        std::cout << "some error in TermAST::printtmD()" << std::endl;
    }
}
/*
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
*/
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
    if(ID == AppTermID) {
        for (auto &term : Terms) {
            term->toDeBrujin(ctx, env);
        }
    } else if (ID == AbsTermID) {
        env.push_back(Name);
        Term->toDeBrujin(ctx, env);
    } else if (ID == VarID) {
        for (int i = env.size()-1; i >= 0; i--) {
            if (env[i] == Name) {
                DIndex = env.size() - i - 1;
                break;
            } 
        }
        if(DIndex < 0) {
            DIndex = ctx[Name] + env.size();
        }
    } else {
        std::cout << "error in toDeBrujin()\n";
    }
}


void TermAST::shift(int d, int c) {
    if (ID == AppTermID) {
        if(Terms.size() < 2) {
            //std::cout << "error in shift :too few terms\n";
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

