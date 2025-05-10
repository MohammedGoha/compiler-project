#include "scanner.h"
#include <cctype>
#include <iostream>
#include <sstream>
#include <unordered_map>

using namespace std;

// Map keywords to token types
unordered_map<string, TokenType> keywords = {
    {"if", IF_KW}, {"then", THEN_KW}, {"else", ELSE_KW}, {"end", END_KW}, {"repeat", REPEAT_KW}, {"until", UNTIL_KW}, {"read", READ_KW}, {"write", WRITE_KW}};

// Function to check if a character is a special single-character operator/symbol
TokenType getSingleCharToken(char ch)
{
    switch (ch)
    {
    case '+':
        return ADDOP;
    case '-':
        return SUBOP;
    case '*':
        return MULOP;
    case '/':
        return DIVOP;
    case '=':
        return COMPARISONOP;
    case '<':
        return COMPARISONOP;
    case ';':
        return SEMICOLON;
    case '(':
    case ')':
        return PUNCTUATION;
    case '{':
        return LEFTBRACKET;
    case '}':
        return RIGHTBRACKET;
    case ',':
        return COMMA;
    default:
        return UNKNOWN;
    }
}

string tokenTypeToString(TokenType t)
{
    switch (t)
    {
    case IF_KW:
        return "IF_KW";
    case THEN_KW:
        return "THEN_KW";
    case ELSE_KW:
        return "ELSE_KW";
    case END_KW:
        return "END_KW";
    case REPEAT_KW:
        return "REPEAT_KW";
    case UNTIL_KW:
        return "UNTIL_KW";
    case READ_KW:
        return "READ_KW";
    case WRITE_KW:
        return "WRITE_KW";
    default:
        return "UNKNOWN";
    }
}
//  scanner function
std::string runScanner(const std::string &input)
{
    stringstream output;
    size_t i = 0;
    while (i < input.length())
    {
        if (isspace(input[i]))
        {
            if (input[i] == '\n')
            {
                output << "\\n --> NEWLINE\r\n";
            }
            i++;
            continue;
        }

        if (isalpha(input[i]))
        {
            string lexeme;
            while (isalnum(input[i]))
            {
                lexeme += input[i++];
            }
            if (keywords.count(lexeme))
            {
                output << lexeme << "--> " << tokenTypeToString(keywords[lexeme]) << "\r\n";
            }
            else
                output << lexeme << " --> ID\r\n";
        }
        else if (isdigit(input[i]))
        {
            string lexeme;
            while (isdigit(input[i]))
            {
                lexeme += input[i++];
            }
            output << lexeme << " --> NUMBER\r\n"
                   << endl;
        }
        else if (input[i] == ':' && input[i + 1] == '=')
        {
            output << ":= --> ASSIGNMENTOP\r" << endl;
            i += 2;
        }
        else if (input[i] == '"')
        {
            string lexeme;
            lexeme += input[i++];
            // handle string literal
            while (i < input.length() && input[i] != '"')
            {
                lexeme += input[i++];
            }

            // don't forget to add one in the next line lexeme += input[i++];
            if (i < input.length())
                lexeme += input[i++];
            output << lexeme << " --> STRING_LITERAL\r\n";
        }
        else
        {
            TokenType tokenType = getSingleCharToken(input[i]);
            bool flag = false;
            if (tokenType != UNKNOWN)
            {
                output << input[i] << " --> ";
                switch (tokenType)
                {
                case ADDOP:
                    output << "ADDOP" << endl;
                    break;
                case SUBOP:
                    output << "SUBOP" << endl;
                    break;
                case MULOP:
                    output << "MULOP" << endl;
                    break;
                case DIVOP:
                    output << "DIVOP" << endl;
                    break;
                case COMPARISONOP:
                    output << "COMPARISONOP" << endl;
                    break;
                case SEMICOLON:
                    output << "SEMICOLON" << endl;
                    break;
                case PUNCTUATION:
                    output << "PUNCTUATION" << endl;
                    break;
                case LEFTBRACKET:
                    output << "LEFTBRACKET" << endl;
                    break;
                case RIGHTBRACKET:
                    output << "RIGHTBRACKET" << endl;
                    // the commented is handel comment
                    /*
                      if (input[i] == '{') {
                          i++; // skip the opening {
                          string lexeme;
                          while (i < input.length() && input[i] != '}') {
                              lexeme += input[i++];
                          }
                          output << "\r\n" << lexeme << " --> Comment Statment" << endl;
                          i--;
                      }
                      */
                    break;
                case COMMA:
                    output << "COMMA" << endl;
                    break;
                default:
                    break;
                }
                output << "\r\n"
                       << endl;

                i++;
            }
            else
            {
                output << input[i] << " --> UNKNOWN\r\n";
                i++;
            }
        }
    }
    return output.str();
}
