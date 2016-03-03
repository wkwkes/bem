
#include "lexer.hpp"

TokenStream *LexicalAnalysis(std::string input_filename) {
    TokenStream *tokens = new TokenStream();
    std::ifstream ifs;
    std::string cur_line;
    std::string token_str;
    int line_num = 0;
    bool iscomment = false;

    ifs.open (input_filename.c_str(), std::ios::in);
    if(!ifs) return NULL;

    while (ifs && getline(ifs, cur_line)) {
        char next_char;
        std::string line;
        Token *next_token;
        int index = 0;
        int length = cur_line.length();

        while (index < length) {
            next_char = cur_line.at(index++);
            if (next_char == EOF) {
                token_str = EOF;
                next_token = new Token(token_str, TOK_EOF, line_num);
            } else if (isspace(next_char)) {
                continue;
            } else if (isalpha(next_char)) {
                while (isalnum(next_char)) {
                    token_str += next_char;
                    if (index == length) {
                        break;
                    }
                    next_char = cur_line.at(index++);
                }
                if (!isalnum(next_char)) {
                    index--;
                }
                if (token_str == "lambda") {
                    next_token = new Token(token_str, TOK_LAM, line_num);
                } else {
                    next_token = new Token (token_str, TOK_ID, line_num);
                }
            } else {
                if (next_char == '(') {
                    token_str += next_char;
                    next_token = new Token(token_str, TOK_LPA, line_num);
                } else if (next_char == ')') {
                    token_str += next_char;
                    next_token = new Token(token_str, TOK_RPA, line_num);
                } else if (next_char == '\\') {
                    token_str += next_char;
                    next_token = new Token(token_str, TOK_LAM, line_num);
                } else if (next_char == '.') {
                    token_str += next_char;
                    next_token = new Token(token_str, TOK_DOT, line_num);
                } else {
                    std::cout << "lex error" << std::endl;
                }
            }
        tokens->pushToken(next_token);
        token_str.clear();
        }
        token_str.clear();
        line_num++;
    }

    if(ifs.eof()) {
        tokens->pushToken(new Token(token_str, TOK_EOF, line_num));
    }

    ifs.close();
    return tokens;
}

TokenStream::~TokenStream() {
    for (int i = 0; i < Tokens.size(); i++) {
        SAFE_DELETE(Tokens[i]);
    }
    Tokens.clear();
}

Token TokenStream::getToken() {
    return *Tokens[CurIndex];
}

bool TokenStream::nextToken() {
    int size = Tokens.size();
    if(--size == CurIndex) {
        return false;
    } else if (CurIndex < size) {
        CurIndex++;
        return true;
    } else {
        return false;
    }
}

bool TokenStream::ungetToken(int times) {
    for (int i = 0; i < times; i++) {
        if (CurIndex == 0) {
            return false;
        } else {
            CurIndex--;
        }
    }
    return true;
}

bool TokenStream::printTokens() {
    std::vector<Token*>::iterator titer = Tokens.begin();
    while (titer != Tokens.end()) {
        fprintf(stdout, "%d:", (*titer)->getTokenType());
        if ((*titer)->getTokenType() != TOK_EOF) {
            fprintf(stdout, "%s\n", (*titer)->getTokenString().c_str());
        }
        titer++;
    }
    return true;
}

