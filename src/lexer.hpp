#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "APP.hpp"

enum TokenType {
    TOK_LAM,
    TOK_ID,
    TOK_DOT,
    TOK_LPA,
    TOK_RPA,
    TOK_EOF,
    TOK_DUM
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
        std::map<std::string, int> Ctx;
        int CurCtxIndex;
    public:
        TokenStream() : CurIndex(0), CurCtxIndex(1) {}
        ~TokenStream();

        bool ungetToken(int Times=1);
        bool nextToken();
        bool pushToken(Token *token) {
            Tokens.push_back(token);
            if (token->getTokenType() == TOK_ID &&
                    Ctx[token->getTokenString()] == 0) {
                Ctx[token->getTokenString()] = CurCtxIndex++;
            }
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
        std::map<std::string, int> &getCtx() {
            return Ctx;
        }
};

TokenStream *LexicalAnalysis(std::string input_filename);
#endif
