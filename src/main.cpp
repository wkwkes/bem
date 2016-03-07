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
//        std::cout<<"始め: ";
  //      parser->getToplevelAST()->getTerm()->PrintDD();
  //      std::cout << "\n";
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
        parser->getToplevelAST()->heval();
   //     std::cout << "\n";
   //     parser->PrintDD();
   //     std::cout << "\n";
        parser->Gen();
        std::cout << "\n";
    } else {
        std::cout << "erroreeeeee\n";
    }
}

