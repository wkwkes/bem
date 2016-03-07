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

void ToplevelAST::toDeBrujin() {
    Term->toDeBrujin(Ctx, *new std::vector<std::string>());
}

void ToplevelAST::Gen() {
    std::cout << "\\documentclass[dvipdfmx]{jsarticle}\n \\usepackage{tikz-qtree}\n \\begin{document}\n \\Tree";
    Term->Gen(Ctx, *new std::vector<std::string>(), true);
    std::cout << "\n\\end{document}";
}

void ToplevelAST::heval() {
    int count = 0;
    while (Term->isEvalable() && count <= 10) {
        Term->apply();
        std::cout << count << " : ";
        Term->PrintDD();
        std::cout << "\n";
        count++;
        if(Term->isEvalable())std::cout << "asdffaf\n";
        std::cout <<"-----------------------------\n";
        //Term->apply();
        std::cout << count << " : ";
        Term->PrintDD();
        std::cout << "\n";
        count++;
        if(Term->isEvalable())std::cout << "asdffaf\n";
    }   
}

/*
void TermAST::Print() {
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

TermAST::TermAST(const TermAST& term) : BaseAST(term.ID), Name(term.Name), DIndex(term.DIndex), Ctx(term.Ctx) {
    for (auto itr : term.Terms) {
        Terms.push_back(new TermAST(*itr));
    }
    if (term.Term != NULL) {
        Term = new TermAST(*(term.Term));
    }
}

TermAST& TermAST::operator=(const TermAST& term) {
    BaseAST(term.ID);
    Name = term.Name;
    DIndex = term.DIndex;
    Ctx = term.Ctx;
    Terms.clear();
    for (auto itr : term.Terms) {
        Terms.push_back(new TermAST(*itr));
    }
    delete Term;
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
        std::cout << "\\"<<".";
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
    //std::cout << " from(" << ID<<") ";
    //PrintDD();   
    //std::cout << " to ";
    if (ID == AppTermID) {
        if(Terms.size() < 2) {
            //std::cout << "error in shift :too few terms\n";
            //std::cout << ID <<std::endl;
            //Terms[0]->PrintDD();
            //PrintDD();
            /*
            for(auto itr : Terms) {
                itr->Print();
                std::cout << std::endl;
            }*/
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
    //PrintDD();
    //std::cout << "\n";
}

void TermAST::subst(int var, TermAST *term) {//[t1 -> t2]this
    std::cout << "start subst\n";
    if (ID == AbsTermID) {
    //std::cout << "go shift!:(subst) ";
    //term->PrintDD();
    //std::cout << "(ID:"<<ID<<")";
    //std::cout << "\n";
        term->shift(1, 0);
    //std::cout << "\n";

    //std::cout << "end shift!\n";
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
            SAFE_DELETE(Term);
            Term = new TermAST(*(term->Term));
            Terms.clear();
            for (auto itr : term->Terms) {
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
    std::cout << "apply: "<<"ID("<<ID<<"), "<<"Terms.size()("<<Terms.size()<<")\n";
    for (int i = 0; i < Terms.size(); i++) {
        std::cout << "Terms["<<i<<"] : ";
        Terms[i]->PrintDD();
        std::cout<<std::endl;
    }
    if(Terms.size() < 2) {
        std::cout << "error: too few terms\n";
        return;
    }   
    if(ID != AppTermID || Terms[0]->getValueID() != AbsTermID) {
        std::cout << "error: cannot apply these terms\n";
        return;
    }   
 //   std::cout << "go shift : (apply1)\n";
    //PrintDD();
    Terms[1]->shift(1, 0); 
    Terms[1]->PrintDD();
    //std::cout << "\n";
    //std::cout << "end shift : (apply1)\n";
    Terms[0]->Term->PrintDD();
    std::cout <<"\n";
    Terms[0]->Term->subst(0, Terms[1]);
    std::cout << "end subst\n";
    std::cout << "Terms[0]\n";
    Terms[0]->Term->PrintDD();
    if(Terms.size()>=2) {
        std::cout << "Terms[1]\n";
        Terms[1]->Term->PrintDD();
    }
    std::cout <<"\n";

    //Terms[0]->PrintDD();
    //std::cout << 114514<<std::endl;
//    Terms[0]->Term->PrintDD();
//    std::cout <<810<< std::endl;
//    std::cout << "go shift : (apply2)\n";
//    PrintDD();
    std::cout << "kokomade ha kiteru!!!!!!!\n";
    std::cout << "kokomade ha kiteru!!!!!!!???????????\n";
    Terms[0]->Term->PrintDD();
    Terms[0]->Term->shift(-1, 0); 
//    std::cout << "\n";
//    std::cout << "end shift : (apply2)\n";
    std::cout << "kokomade ha kiteru!!!!!!!???????????\n";
    Terms[0] = Terms[0]->Term;
//    SAFE_DELETE(Terms[0]);
    //Terms[0] = new TermAST(*(Terms[0]->Term)); 
    std::cout << "kokomade ha kiteru!!!!!!!???????????\n";
    Terms.erase(Terms.begin()+1);
    std::cout << "kokomade ha kiteru!!!!!!!???????????\n";
    if (Terms[0]->getValueID() == AppTermID &&
            Terms[0]->Terms.size() == 1) {
        std::cout << "yatteruzpoi ! ! ! !\n";
        Terms[0]->ID = Terms[0]->Terms[0]->getValueID();
        Terms[0]->DIndex = Terms[0]->Terms[0]->DIndex;
        Terms[0]->Name = Terms[0]->Terms[0]->Name;
        std::cout << "''''''''''''''''''''''''''''\n";
        if (Terms[0]->Terms[0]->Term != NULL) {
            Terms[0]->Term = new TermAST(*(Terms[0]->Terms[0]->Term));
            std::cout << "''''''''''''''''''''''''''''\n";
        } else {
            Terms[0]->Term = NULL;
        }
        Terms[0]->Terms.clear();
        std::cout << "''''''''''''''''''''''''''''\n";
        for (auto itr : Terms[0]->Terms[0]->Terms) {
            Terms[0]->Terms.push_back(new TermAST(*itr));
        }
    }
    std::cout << "Terms[0]\n";
    Terms[0]->Term->PrintDD();
    if(Terms.size()>=2) {
        std::cout << "Terms[1]\n";
        Terms[1]->Term->PrintDD();
    }
    std::cout <<"\n";/*
    if (Terms.size() == 1) {
        std::cout << "yatteruzpoi ! ! ! !\n";
        ID = Terms[0]->getValueID();
        DIndex = Terms[0]->DIndex;
        Name = Terms[0]->Name;
        Term = new TermAST(*(Terms[0]->Term));
        Terms.clear();
        for (auto itr : Terms[0]->Terms) {
            Terms.push_back(new TermAST(*itr));
        }
    }*/
    std::cout << "korega saigo ^^^^^^~~~~~~~~~~~~~\n";
    Terms[0]->PrintDD();
    std::cout << "korega saigo ^^^^^^~~~~~~~~~~~~??????~\n";

    return;
}

void TermAST::Gen(std::map<std::string, int> &ctx, std::vector<std::string> env, bool top) {
    if (ID == AbsTermID) {
        env.push_back(pickfresh(ctx, env, Name));
        if (top) {
            std::cout << "[. ";
        }
        std::cout << "{ $\\lambda "<<env[env.size() - 1] << ". $ ";
        if(Term->getValueID() == AppTermID) {
            if (Term->Terms[0]->getValueID() == VarID) {
                Term->Terms[0]->Gen(ctx, env, false);
                Term->Terms.erase(Term->Terms.begin());
            }
        } else if (Term->getValueID() == VarID) {
            Term->Gen(ctx, env, false);
        }
        std::cout << "}";
        //Term->Gen(ctx, env, false);
        if (top) {
            std::cout << " ] ";
        }
    } else if (ID == AppTermID) {
        std::cout << " [. ";
        if (top) {
            std::cout << " { } ";
        }
        for (int i = 0; i < Terms.size(); i++) {
            if(Terms.at(i)->getValueID() != VarID) {
                std::cout << " [. ";
                Terms.at(i)->Gen(ctx, env, false);
                std::cout << " ] ";
            } else {
                Terms.at(i)->Gen(ctx, env, false);
                if(i != Terms.size()-1) {
                    std::cout <<" ";
                }
            }
        }
        std::cout << " ] ";
    } else if (ID == VarID) {
        if (DIndex < env.size()) {
            std::cout <<"$" << env[env.size()-1-DIndex] <<"$";
        } else {
            int index = DIndex - env.size();
            for (auto itr : ctx) {
                if (itr.second == index) {
                    std::cout <<itr.first;
                }
            }
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
