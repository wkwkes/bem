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
void ToplevelAST::PrintDD() {
    Term->PrintDD();
}

void ToplevelAST::PrintL() {
    Term->PrintL();
}

void ToplevelAST::toDeBrujin() {
    Term->toDeBrujin(Ctx, *new std::vector<std::string>());
}

void ToplevelAST::Gen() {
    Term->Gen(Ctx, *new std::vector<std::string>(), true);
    std::cout << "\n\\end{document}";
}

void ToplevelAST::heval() {
    int count = 0;
    while (count <= 100) {
        Term->hbeta(0);
        count++;
    }   
}

void TermAST::hbeta(int times) {
    times++;
    if (times >= 50) {
        return;
    }
    if (ID == VarID) {
        return;
    }
    if (ID == AppTermID) {
        if (Terms[0]->getValueID() == VarID) {
            if (Terms.size() == 1) {
                return;
            } else {
                for(auto itr : Terms) {
                    itr->hbeta(times);
                }
            }
        }
        if (Terms[0]->getValueID() == AbsTermID) {
            if (Terms.size() >= 2) {
                apply();
            } else {
                Terms[0]->hbeta(times);
                return;
            }
            hbeta(times);
        }
        if (Terms[0]->getValueID() == AppTermID) {
            for (int i = 1; i < Terms.size(); i++) {
                Terms[0]->Terms.push_back(Terms[i]);
            }
            Terms = Terms[0]->Terms;
            hbeta(times);
        }
    }
    if (ID == AbsTermID) {
        Term->hbeta(times);
    }
}


TermAST::TermAST(const TermAST& term) : BaseAST(term.ID), Name(term.Name), DIndex(term.DIndex), Ctx(term.Ctx) {
    for (auto itr : term.Terms) {
        Terms.push_back(new TermAST(*itr));
    }
    if (term.Term != NULL) {
        Term = new TermAST(*(term.Term));
    } else {
        Term = NULL;
    }
}

TermAST& TermAST::operator=(const TermAST& term) {
    BaseAST(term.ID);
    Name = term.Name;
    DIndex = term.DIndex;
    Ctx = term.Ctx;
    for (auto itr : Terms) {
        SAFE_DELETE(itr);
    }
    Terms.clear();
    for (auto itr : term.Terms) {
        Terms.push_back(new TermAST(*itr));
    }
    SAFE_DELETE(Term);
    Term = new TermAST(*(term.Term));
    return *this;
}

std::string TermAST::pickfresh(std::map<std::string, int> &ctx, std::vector<std::string> env, std::string str) {
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
        for (auto itr : ctx) {
            if (itr.first == str) {
                str+="'";
                ch = true;
                break;
            }
        }
        
    }
    return str;
}

void TermAST::Printtm(std::map<std::string, int> &ctx, std::vector<std::string> env) {
    std::cout << "<"<<getValueID()<<">";
    if (ID == AbsTermID) {
        env.push_back(pickfresh(ctx, env, Name));
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
        env.push_back(pickfresh(ctx, env, Name));
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
void TermAST::PrintDD() {
    std::cout << "<"<<ID<<">"; 
    if (ID == AbsTermID) {
        std::cout << "\\"<<Name<<".";
        Term->PrintDD();
    } else if (ID == AppTermID) {
        for (int i = 0; i < Terms.size(); i++) {
            if(Terms.at(i)->getValueID() != VarID) {
                std::cout << "(";
                Terms.at(i)->PrintDD();
                std::cout << ")";
            } else {
                Terms.at(i)->PrintDD();
                if(i != Terms.size()-1) {
                    std::cout <<" ";
                }
            }
        }
    } else if (ID == VarID) {
        std::cout <<"{"<<DIndex <<","<<Name<<"}";
    } else {
        std::cout << "some error in TermAST::print()" << std::endl;
    }
}

void TermAST::PrintL() {
    if (ID == AbsTermID) {
        std::cout << "$\\lambda$"<<Name<<".";
        Term->PrintL();
    } else if (ID == AppTermID) {
        for (int i = 0; i < Terms.size(); i++) {
            if(Terms.at(i)->getValueID() != VarID) {
                std::cout << "(";
                Terms.at(i)->PrintL();
                std::cout << ")";
            } else {
                Terms.at(i)->PrintL();
                if(i != Terms.size()-1) {
                    std::cout <<" ";
                }
            }
        }
    } else if (ID == VarID) {
        std::cout <<Name;
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
        for (auto itr : Terms) {
            itr->shift(d, c);
        }
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

void TermAST::subst(int var, TermAST term) {//[t1 -> t2]this
    if (ID == AbsTermID) {
        term.shift(1, 0);
        Term->subst(var + 1, term);
        return;
    } else if (ID == AppTermID) {
        for (auto t : Terms) {
            t->subst(var, term);
        }
        return;
    } else if (ID == VarID) {
        if (DIndex == var) {
            ID = term.ID;
            Name = term.Name;
            DIndex = term.DIndex;
            SAFE_DELETE(Term);
            if(term.Term != NULL) {
                Term = new TermAST(*(term.Term));
            } else {
                Term = NULL;
            }
            Terms.clear();
            for (auto itr : term.Terms) {
                Terms.push_back(new TermAST(*itr));
            }
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
    Terms[0]->Term->subst(0, *Terms[1]);
    Terms[0]->Term->shift(-1, 0); 
    if (Terms[0]->Term != NULL) { 
        Terms[0] = Terms[0]->Term;
    } else {
        Terms[0] = NULL;
    }
    Terms.erase(Terms.begin()+1);
    if (Terms[0]->getValueID() == AppTermID &&
            Terms[0]->Terms.size() == 1) {
        Terms[0]->ID = Terms[0]->Terms[0]->getValueID();
        Terms[0]->DIndex = Terms[0]->Terms[0]->DIndex;
        Terms[0]->Name = Terms[0]->Terms[0]->Name;
        if (Terms[0]->Terms[0]->Term != NULL) {
            SAFE_DELETE(Terms[0]->Term);
            Terms[0]->Term = new TermAST(*(Terms[0]->Terms[0]->Term));
        } else {
            Terms[0]->Term = NULL;
        }
        Terms[0]->Terms = Terms[0]->Terms[0]->Terms;
    }
    if (Terms.size() == 1 && ID == AppTermID) {
        ID = Terms[0]->getValueID();
        DIndex = Terms[0]->DIndex;
        Name = Terms[0]->Name;
        if (Terms[0]->Term != NULL) {
            Term = new TermAST(*(Terms[0]->Term));
        } else {
            Term = NULL;
        }
        Terms = Terms[0]->Terms;
    }
    return;
}


void TermAST::Gen(std::map<std::string, int> &ctx, std::vector<std::string> env, bool top) {
    if (ID == AbsTermID) {
        //bool ch = Term->Terms[0]->getValueID() == VarID && Term->Terms.getValueID() == AppTermID;
        env.push_back(pickfresh(ctx, env, Name));
        std::cout << "[. ";
        std::cout << "{ $\\lambda "<<env[env.size() - 1];
        while(Term->Terms[0]->getValueID() == AbsTermID && Term->Terms.size() == 1) {
            env.push_back(pickfresh(ctx, env, Term->Terms[0]->getVar()));
            std::cout << " "<< env[env.size() - 1];
            Term = Term->Terms[0]->Term;
        }
        std::cout << ". $ ";
        if(Term->getValueID() == AppTermID) {
            if (Term->Terms[0]->getValueID() == VarID) {
                Term->Terms[0]->Gen(ctx, env, false);
                Term->Terms.erase(Term->Terms.begin());
            }
        }
        std::cout << " } ";
        Term->Gen(ctx, env, false);
        std::cout << " ] ";
    } else if (ID == AppTermID) {
        if (Terms.size() == 1) {
            Terms[0]->Gen(ctx, env, false);
            return;
        }
        if (Terms[0]->getValueID() == VarID) {
            std::cout << "[. ";
            //std::cout << " [. ";
            Terms[0]->Gen(ctx, env, false);
            std::cout << " ";
            for (int i = 1; i < Terms.size(); i++) {
                Terms.at(i)->Gen(ctx, env, false);
                std::cout << " ";
            }
            std::cout << " ] ";
        } else { 
            if (top) {
                std::cout << "[. { } ";
            }
            //std::cout << " { } ";
            for (int i = 0; i < Terms.size(); i++) {
                Terms.at(i)->Gen(ctx, env, false);
            }
            if (top) {
                std::cout << " ] ";
            }
        }
    } else if (ID == VarID) {
        if (top) {
            std::cout << "[. ";
        }
        if (DIndex < env.size()) {
            std::cout <<"$" << env[env.size()-1-DIndex] <<"$";
        } else {
            int index = DIndex - env.size();
            for (auto itr : ctx) {
                if (itr.second == index) {
                    std::cout <<itr.first;
                    break;
                }
            }
        }
        if (top) {
            std::cout << " ] ";
        }
    } else {
        std::cout << "some error in TermAST::print()" << std::endl;
    }
}

bool TermAST::isEvalable() {
    if (ID == AppTermID && Terms.size() >= 2) {
        if (Terms[0]->getValueID() == AbsTermID && Terms.size() >= 2) {
            return true;
        }   
    }   
    return false;
}
