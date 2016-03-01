#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include "APP.hpp"

enum TokenType {
    TOK_LAM,
    TOK_ID,
    TOK_DOT,
    TOK_LPA,
    TOK_RPA,
    TOK_EOF
};


typedef class Token {
    private:
        TokenType Type;
        std::string TokenString;
        int Line;

    public:
        Token(std::string str, TokenType type, int line)
            : TokenString(str), Type(type), Line(line) {};
        ~Token() {};

        TokenType getTokenType() {return Type;};
        std::string getTokenString() {return TokenString;};
        bool setLine (int line) {Line = line; return true;}
        int getLine() {return Line;};
} Token;

class TokenStream {
    private:
        std::vector<Token*> Tokens;
        int CurIndex;

    public:
        TokenStream() : CurIndex(0) {};
        ~TokenStream();

        bool ungetToken(int Times=1);
        bool nextToken();
        bool pushToken(Token *token) {
            Tokens.push_back(token);
            return true;
        }
        Token getToken();
        TokenType getCurType() {
            return Tokens[CurIndex]->getTokenType();
        }
        std::string getCurString() {
            return Tokens[CurIndex]->getTokenString();
        }
        bool printTokens();
        int getCurIndex() {
            return CurIndex;
        }
        bool applyTokenIndex(int index) {
            CurIndex = index;
            return true;
        }
};

TokenStream *LexicalAnalysis(std::string input_filename);
#endif
