#ifndef SCANNER_H
#define SCANNER_H


#include <string>

// The scanner function that takes input string and returns the result as string
std::string runScanner(const std::string& input);

// Token types
enum TokenType {
    IF_KW, THEN_KW, ELSE_KW, END_KW, REPEAT_KW, UNTIL_KW, READ_KW, WRITE_KW,
    ID, NUMBER, ASSIGNMENTOP, STRING_LITERAL, ADDOP, SUBOP, MULOP, DIVOP,
    COMPARISONOP, SEMICOLON, PUNCTUATION, LEFTBRACKET, RIGHTBRACKET, COMMA,
    NEWLINE, UNKNOWN
};


#endif
