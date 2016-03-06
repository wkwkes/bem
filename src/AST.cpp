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
 //   std::cout << "go shift : (apply1)\n";
    //PrintDD();
    //Terms[1]->shift(1, 0); 
    //std::cout << "\n";
    //std::cout << "end shift : (apply1)\n";
    Terms[0]->Term->subst(0, Terms[1]);
    //Terms[0]->PrintDD();
    //std::cout << 114514<<std::endl;
//    Terms[0]->Term->PrintDD();
//    std::cout <<810<< std::endl;
//    std::cout << "go shift : (apply2)\n";
//    PrintDD();
    Terms[0]->Term->shift(-1, 0); 
//    std::cout << "\n";
//    std::cout << "end shift : (apply2)\n";
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

void TermAST::Gen(std::map<std::string, int> &ctx, std::vector<std::string> env, bool top) {
    if (ID == AbsTermID) {
        env.push_back(pickfresh(ctx, env, Name));
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
        Term->Gen(ctx, env, false);
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
