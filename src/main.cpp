#include "lexer.hpp"
#include "AST.hpp"
#include "parser.hpp"



int main(int argc, char **argv) {
 //   std::cout<< "starts\n";
    Parser *parser = new Parser(argv[1]);
    if (parser->doParse()) {
        parser->getToplevelAST()->toDeBrujin();
   //     parser->PrintDD();
    //    std::cout<< "\n";
        std::cout<<"始め: ";
        parser->getToplevelAST()->getTerm()->PrintDD();
        std::cout << "\n";
//        parser->getToplevelAST()->getTerm()->apply();
    //    std::cout << "^^^^\n";
 //       parser->getToplevelAST()->getTerm()->PrintDD();
   //     std::cout << "\n^^^^\n";
        //parser->getToplevelAST()->getTerm()->apply();
        //parser->getToplevelAST()->heval();
//        std::cout << "\n-------------------------------\n";
        
    //    parser->getToplevelAST()->getTerm()->apply();
    //    std::cout << "^^^^\n";
      //  parser->getToplevelAST()->getTerm()->PrintDD();
   //     std::cout << "\n^^^^\n";
        //parser->getToplevelAST()->getTerm()->apply();
   //     std::cout << "\n";
   //     parser->PrintDD();
   //     std::cout << "\n";
        std::cout << "\\documentclass[dvipdfmx]{jsarticle}\n \\usepackage{tikz-qtree,amsmath,amssymb,amsfonts}\n \\begin{document}\n";
        parser->getToplevelAST()->getTerm()->PrintL();
        std::cout<<"\n\n\\hspace{20pt}\n$\\underset{\\small{\\beta}}{\\twoheadrightarrow}$\n\n";
//        parser->getToplevelAST()->heval();
        parser->getToplevelAST()->getTerm()->hbeta();
        parser->getToplevelAST()->getTerm()->PrintDD();
        std::cout << std::endl;
        parser->getToplevelAST()->getTerm()->PrintL();
        std::cout << "\n\n\\Tree ";
        parser->Gen();
        std::cout << "\n";
    } else {
        std::cout << "erroreeeeee\n";
    }
}

