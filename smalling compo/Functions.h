#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <stdexcept>


// Token types
enum TokenType2 {
    IF_KW, THEN_KW, ELSE_KW, END_KW, REPEAT_KW, UNTIL_KW, READ_KW, WRITE_KW,
    ID, NUMBER, ASSIGNMENTOP, STRING_LITERAL, ADDOP, SUBOP, MULOP, DIVOP,
    COMPARISONOP, SEMICOLON, PUNCTUATION, LEFTBRACKET, RIGHTBRACKET, COMMA,
    NEWLINE, UNKNOWN
};

// Token Type
struct scannerToken {
    TokenType2 type;
    std::string value;
    int line;
};

// The scanner function that takes input string and returns the result as string
std::string runScanner(const std::string& input);

// Tokenization function to convert scanner string to a vector of tokens
std::vector<scannerToken> tokenize(const std::string& tokenString);

// Program parser
std::string parseProgram(const std::vector<scannerToken>& tokens);

void parseIf(const std::vector<scannerToken>& tokens, size_t& currTokenIndex);
bool parseCondition(const std::vector<scannerToken>& tokens, size_t& currTokenIndex);
bool parseExpression(const std::vector<scannerToken>& tokens, size_t& currTokenIndex);
void parseWrite(const std::vector<scannerToken>& tokens, size_t& currTokenIndex);
void parseRead(const std::vector<scannerToken>& tokens, size_t& currTokenIndex);
void parseRepeat(const std::vector<scannerToken>& tokens, size_t& currTokenIndex);
void parseComment(const std::vector<scannerToken>& tokens, size_t& currTokenIndex);
void parseAssignment(const std::vector<scannerToken>& tokens, size_t& currTokenIndex);
bool lookahead(const std::vector<scannerToken>& tokens, size_t& currTokenIndex, TokenType2 type);
bool jumpTo(const std::vector<scannerToken>& tokens, size_t& currTokenIndex, TokenType2 Token);
bool advanceToken(const std::vector<scannerToken>& tokens, size_t& currTokenIndex);

#endif
