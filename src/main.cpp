#include "lexer.hpp"
#include "AST.hpp"
#include "parser.hpp"



int main(int argc, char **argv) {
    Parser *parser = new Parser(argv[1]);
    if (parser->doParse()) {
        parser->getToplevelAST()->toDeBrujin();
        std::cout << "\\documentclass[dvipdfmx]{jsarticle}\n \\usepackage{tikz-qtree,amsmath,amssymb,amsfonts}\n \\begin{document}\n";
        parser->getToplevelAST()->getTerm()->PrintL();
        std::cout<<"\n\n\\hspace{20pt}\n$\\underset{\\small{\\beta}}{\\twoheadrightarrow}$";
        parser->getToplevelAST()->heval();
        std::cout << "\n\n\\Tree ";
        parser->Gen();
        std::cout << "\n";
    } else {
        std::cout << "error\n";
    }
}

