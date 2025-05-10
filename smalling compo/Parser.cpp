#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <map>

enum TokenType {
    IF_KW, THEN_KW, ELSE_KW, END_KW, REPEAT_KW, UNTIL_KW, READ_KW, WRITE_KW,
    ID, NUMBER, ASSIGNMENTOP, STRING_LITERAL, ADDOP, SUBOP, MULOP, DIVOP,
    COMPARISONOP, SEMICOLON, PUNCTUATION, LEFTBRACKET, RIGHTBRACKET, COMMA,
    NEWLINE, UNKNOWN
};
struct Token {
    TokenType type;
    std::string value;
    int line;
};

// Mapping for string tokens to enum types
std::map<std::string, TokenType> tokenMap = {
    {"IF_KW", IF_KW}, {"THEN_KW", THEN_KW}, {"ELSE_KW", ELSE_KW},
    {"END_KW", END_KW}, {"REPEAT_KW", REPEAT_KW}, {"UNTIL_KW", UNTIL_KW},
    {"READ_KW", READ_KW}, {"WRITE_KW", WRITE_KW}, {"ID", ID},
    {"NUMBER", NUMBER}, {"ASSIGNMENTOP", ASSIGNMENTOP}, {"STRING_LITERAL", STRING_LITERAL},
    {"ADDOP", ADDOP}, {"SUBOP", SUBOP}, {"MULOP", MULOP}, {"DIVOP", DIVOP},
    {"COMPARISONOP", COMPARISONOP}, {"SEMICOLON", SEMICOLON},
    {"PUNCTUATION", PUNCTUATION}, {"LEFTBRACKET", LEFTBRACKET}, {"RIGHTBRACKET", RIGHTBRACKET},
    {"COMMA", COMMA}, {"NEWLINE", NEWLINE}, {"UNKNOWN",UNKNOWN}
};
std::map<TokenType, std::string> reverseTokenMap = {
    {IF_KW, "IF_KW"}, {THEN_KW, "THEN_KW"}, {ELSE_KW, "ELSE_KW"},
    {END_KW, "END_KW"}, {REPEAT_KW, "REPEAT_KW"}, {UNTIL_KW, "UNTIL_KW"},
    {READ_KW, "READ_KW"}, {WRITE_KW, "WRITE_KW"}, {ID, "ID"},
    {NUMBER, "NUMBER"}, {ASSIGNMENTOP, "ASSIGNMENTOP"}, {STRING_LITERAL, "STRING_LITERAL"},
    {ADDOP, "ADDOP"}, {SUBOP, "SUBOP"}, {MULOP, "MULOP"}, {DIVOP, "DIVOP"},
    {COMPARISONOP, "COMPARISONOP"}, {SEMICOLON, "SEMICOLON"},
    {PUNCTUATION, "PUNCTUATION"}, {LEFTBRACKET, "LEFTBRACKET"}, {RIGHTBRACKET, "RIGHTBRACKET"},
    {COMMA, "COMMA"}, {NEWLINE, "NEWLINE"}, {UNKNOWN, "UNKNOWN"}
};
std::vector<std::string> errors;

void error(const std::string& message, const int& line) {
    errors.push_back("Line " + std::to_string(line) +": " + message);
}

std::vector<Token> tokenize(const std::string& tokenString){

    std::vector<Token> tokens;
    std::istringstream stream(tokenString);
    std::string tokenStr;
    int line = 1;


    while (stream >> tokenStr) {
        
        if (tokenStr == "NEWLINE") {
            line++;
            continue;
        }
        
        Token token;
        token.value = tokenStr;
        token.line = line;
        token.type = tokenMap.at(tokenStr);

        tokens.push_back(token);

    }

    return tokens;
}

void parseIf(const std::vector<Token>& tokens, size_t& currTokenIndex);
bool parseCondition(const std::vector<Token>& tokens, size_t& currTokenIndex);
bool parseExpression(const std::vector<Token>& tokens, size_t& currTokenIndex);
void parseWrite(const std::vector<Token>& tokens, size_t& currTokenIndex);
void parseRead(const std::vector<Token>& tokens,size_t& currTokenIndex);
void parseRepeat(const std::vector<Token>& tokens, size_t& currTokenIndex);
void parseComment(const std::vector<Token>& tokens, size_t& currTokenIndex);
void parseAssignment(const std::vector<Token>& tokens, size_t& currTokenIndex);
bool lookahead(const std::vector<Token>& tokens, size_t& currTokenIndex, TokenType type);
bool jumpTo(const std::vector<Token>& tokens, size_t& currTokenIndex, TokenType Token);
bool advanceToken(const std::vector<Token>& tokens, size_t& currTokenIndex);

bool advanceToken(const std::vector<Token>& tokens, size_t& currTokenIndex) {
    currTokenIndex++;
    return currTokenIndex >= tokens.size();
}

bool jumpTo(const std::vector<Token>& tokens, size_t& currTokenIndex, TokenType Token) {
    size_t temp = currTokenIndex;
    while (temp < tokens.size() && tokens[temp].type != Token) {
        temp++;
    }
    if (temp < tokens.size()) {
        currTokenIndex = temp;
    }
    else {
        return false;
    }
    return true;
}

bool lookahead(const std::vector<Token>& tokens, size_t& currTokenIndex, TokenType type) {

    for (size_t temp = currTokenIndex; temp < tokens.size(); temp++) {
        if (tokens[temp].type == type) {
            return true;
        }
    }
    return false;
}

void parseStatement(const std::vector<Token>& tokens, size_t& currTokenIndex) {

    switch (tokens[currTokenIndex].type) {
    case IF_KW: parseIf(tokens,currTokenIndex); break;
    case WRITE_KW: parseWrite(tokens,currTokenIndex); break;
    case READ_KW: parseRead(tokens,currTokenIndex); break;
    case REPEAT_KW: parseRepeat(tokens,currTokenIndex); break;
    case ID: parseAssignment(tokens, currTokenIndex); break;
    case LEFTBRACKET: parseComment(tokens,currTokenIndex); break;
    default: 
        error("Invalid Token", tokens[currTokenIndex].line); 
        std::cout << reverseTokenMap.at(tokens[currTokenIndex].type)<<std::endl; 
        return;
    }

}

void printindex(size_t& currTokenIndex) {
    std::cout << currTokenIndex << std::endl;
}

void parseComment(const std::vector<Token>& tokens, size_t& currTokenIndex) {

    if (advanceToken(tokens, currTokenIndex)) {
        error("comment not terminated.", tokens[currTokenIndex - 1].line);
        return;
    }

    if (!jumpTo(tokens, currTokenIndex, RIGHTBRACKET)) {
        error("comment not terminated.", tokens[currTokenIndex - 1].line);
        return;
    }

    return;
}

void parseAssignment(const std::vector<Token>& tokens, size_t& currTokenIndex) {

    if (advanceToken(tokens, currTokenIndex)) {
        error("Assignment statement must include assignment operator", tokens[currTokenIndex - 1].line);
        error("Missing semicolon", tokens[currTokenIndex - 1].line);
    }
    
    if (tokens[currTokenIndex].type != ASSIGNMENTOP) {
        error("Assignment statement must include assignment operator", tokens[currTokenIndex - 1].line);
        return;
    }

    if (advanceToken(tokens, currTokenIndex)) {
        error("Missing expression", tokens[currTokenIndex - 1].line);
        return;
    }

    if (!parseExpression(tokens, currTokenIndex)) {
        error("Missing expression", tokens[currTokenIndex - 1].line);
        return;
    }

    if (currTokenIndex >= tokens.size()) {
        error("Missing semicolon.", tokens[currTokenIndex - 1].line);
        return;
    }

    if (tokens[currTokenIndex].type != SEMICOLON) {
        error("Missing semicolon.", tokens[currTokenIndex - 1].line);
        currTokenIndex--;
        return;
    }
    
    return;
}

void parseRepeat(const std::vector<Token>& tokens, size_t& currTokenIndex) {

    if (!lookahead(tokens, currTokenIndex, UNTIL_KW)) {
        error("Missing keyword until", tokens[currTokenIndex].line);
        return;
    }

    if (advanceToken(tokens, currTokenIndex)) {
        error("repeat must be terminated with until", tokens[currTokenIndex - 1].line);
        error("Missing semicolon", tokens[currTokenIndex - 1].line);
        return;
    }

    size_t range = currTokenIndex;
    while (range < tokens.size() && tokens[range].type != UNTIL_KW) {
        range++;
    }

    if (currTokenIndex == range) {
        error("Missing statements.", tokens[currTokenIndex].line);
        error("Missing condition.", tokens[currTokenIndex].line);
        error("Missing semicolon.", tokens[currTokenIndex].line);
        return;
    }

    else {
        for (currTokenIndex; currTokenIndex < range; currTokenIndex++) {
            parseStatement(tokens, currTokenIndex);
        }
    }

    if (advanceToken(tokens, currTokenIndex)) {
        error("Missing condition after 'until'", tokens[currTokenIndex - 1].line);
        error("Missing semicolon", tokens[currTokenIndex - 1].line);
        return;
    }

    if (tokens[currTokenIndex].type == SEMICOLON) {
        error("Missing condition.", tokens[currTokenIndex].line);
        return;
    }

    if (!parseCondition(tokens, currTokenIndex)) {
        error("Invalid Condition.", tokens[currTokenIndex].line);
        if (!jumpTo(tokens, currTokenIndex, SEMICOLON)) {
            error("Missing semicolon.", tokens[currTokenIndex].line);
            return;
        }
    }
   
    if (currTokenIndex >= tokens.size()) {
        error("Missing semicolon.", tokens[currTokenIndex - 1].line);
        return;
    }

    if (tokens[currTokenIndex].type != SEMICOLON) {
        error("Missing semicolon.", tokens[currTokenIndex].line);
        currTokenIndex--;
        return;
    }
        
    return;
}

void parseWrite(const std::vector<Token>& tokens, size_t& currTokenIndex) {

    if (advanceToken(tokens, currTokenIndex)) {
        error("write must be followed by an identifier/string literal", tokens[currTokenIndex - 1].line);
        error("Missing semicolon", tokens[currTokenIndex - 1].line);
        return;
    }

    if (tokens[currTokenIndex].type != ID && tokens[currTokenIndex].type != STRING_LITERAL) {
        error("write must be followed by an identifier/string literal", tokens[currTokenIndex].line);
    }

    if (advanceToken(tokens, currTokenIndex)) {
        error("Missing semicolon", tokens[currTokenIndex - 1].line);
        return;
    }

    while (tokens[currTokenIndex].type == COMMA) {

        if (advanceToken(tokens, currTokenIndex)) {
            error("Expected identifier or string literal after comma", tokens[currTokenIndex-1].line);
            error("Missing semicolon", tokens[currTokenIndex - 1].line);
            return;
        }

        if (tokens[currTokenIndex].type != ID && tokens[currTokenIndex].type != STRING_LITERAL) {
            error("Expected identifier or string literal after comma", tokens[currTokenIndex].line);
            
            if (advanceToken(tokens, currTokenIndex)) {
                error("Missing semicolon", tokens[currTokenIndex - 1].line);
                return;
            }

            continue;
        }
        
        if (advanceToken(tokens, currTokenIndex)) {
            error("Missing semicolon", tokens[currTokenIndex - 1].line);
            return;
        }

    }

    if (tokens[currTokenIndex].type != SEMICOLON) {
        error("Missing semicolon.", tokens[currTokenIndex-1].line);
        currTokenIndex--;
        return;
    }


    return;
}

void parseRead(const std::vector<Token>& tokens, size_t& currTokenIndex) {

    if (advanceToken(tokens, currTokenIndex)) {
        error("read must be followed by an identifier", tokens[currTokenIndex - 1].line);
        error("Missing semicolon", tokens[currTokenIndex - 1].line);
        return;
    }

    if (tokens[currTokenIndex].type != ID) {
        error("read must be followed by an identifier", tokens[currTokenIndex].line);
    }

    if (advanceToken(tokens, currTokenIndex)) {
        error("Missing semicolon", tokens[currTokenIndex - 1].line);
        return;
    }

    while (tokens[currTokenIndex].type == COMMA) {

        if (advanceToken(tokens, currTokenIndex)) {
            error("Expected an identifier after comma", tokens[currTokenIndex-1].line);
            error("Missing semicolon", tokens[currTokenIndex - 1].line);
            return;
        }

        if (tokens[currTokenIndex].type != ID) {
            error("Expected an identifier after comma.", tokens[currTokenIndex].line);
            
            if (advanceToken(tokens, currTokenIndex)) {
                error("Missing semicolon", tokens[currTokenIndex - 1].line);
                return;
            }

            continue;
        }
        
        if (advanceToken(tokens, currTokenIndex)) {
            error("Missing semicolon", tokens[currTokenIndex - 1].line);
            return;
        }

    }

    if (tokens[currTokenIndex].type != SEMICOLON) {
        error("Missing semicolon.", tokens[currTokenIndex-1].line);
        currTokenIndex--;
        return;
    }


    return;
}

void parseIf(const std::vector<Token>& tokens, size_t& currTokenIndex) {

    if (advanceToken(tokens,currTokenIndex)) {
        error("Invald Condition.", tokens[currTokenIndex - 1].line);
        return;
    }
    
    if (!parseCondition(tokens, currTokenIndex)) {
        error("Invalid Condition.", tokens[currTokenIndex].line);
        if (!jumpTo(tokens, currTokenIndex, THEN_KW)) {
            error("Missing keyword then", tokens[currTokenIndex].line);
        }
    }
    
    if (!lookahead(tokens, currTokenIndex, THEN_KW)) {
        error("Missing keyword then", tokens[currTokenIndex - 1].line);
        currTokenIndex--;
        return;
    }


    if (tokens[currTokenIndex].type != THEN_KW) {
        error("Invalid Condition.",tokens[currTokenIndex].line);
        if (!jumpTo(tokens, currTokenIndex, THEN_KW)) {
            error("Missing keyword then", tokens[currTokenIndex].line);
        }
    }

    if (advanceToken(tokens, currTokenIndex)) {
        error("Missing if body", tokens[currTokenIndex - 1].line);
        return;
    }

    if (lookahead(tokens, currTokenIndex, ELSE_KW)) {
       
        size_t range = currTokenIndex;
        while (range < tokens.size() && tokens[range].type != ELSE_KW) {
            range++;
        }
        if (currTokenIndex == range) {
            if (advanceToken(tokens, currTokenIndex)) {
                error("Missing keyword end", tokens[currTokenIndex-1].line);
                return;
            }
        }
        else {
            for (currTokenIndex; currTokenIndex < range; currTokenIndex++) {
                parseStatement(tokens, currTokenIndex);
            }
            if (advanceToken(tokens, currTokenIndex)) {
                error("Missing keyword end", tokens[currTokenIndex - 1].line);
                return;
            }
        }

    }

    if (lookahead(tokens, currTokenIndex, END_KW)) {
        size_t range = currTokenIndex;
        while (range < tokens.size() && tokens[range].type != END_KW) {
            range++;
        }
        if (currTokenIndex == range) {
        }
        else {
            for (currTokenIndex; currTokenIndex < range; currTokenIndex++) {
                parseStatement(tokens, currTokenIndex);
            }
        }
    }
    else {
        error("Missing keyword end", tokens[currTokenIndex].line);
        currTokenIndex--;
        return;
    }

    if (advanceToken(tokens, currTokenIndex)) {
        return;
    }

    if (tokens[currTokenIndex].type != SEMICOLON) {//end doesn't necessarily need to be followed by semicolon, can be changed to output error if needed.
        currTokenIndex--;
        return;
    }

}

bool parseCondition(const std::vector<Token>& tokens, size_t& currTokenIndex){

    
    if (!parseExpression(tokens, currTokenIndex)) {
        return false;
    }

    if (currTokenIndex >= tokens.size()) { return false; }

    if (tokens[currTokenIndex].type != COMPARISONOP) {
        return false;
    }
    currTokenIndex++;

    if (!parseExpression(tokens, currTokenIndex)) {
        return false;
    }

    return true;

}

bool parseExpression(const std::vector<Token>& tokens, size_t& currTokenIndex) {

    if (currTokenIndex >= tokens.size()) { return false; }
    
    if (tokens[currTokenIndex].type != ID && tokens[currTokenIndex].type != NUMBER) {
        return false;
    }

    currTokenIndex++;

    while (currTokenIndex < tokens.size() &&
        (tokens[currTokenIndex].type == MULOP || tokens[currTokenIndex].type == DIVOP ||
        tokens[currTokenIndex].type == SUBOP || tokens[currTokenIndex].type == ADDOP)) {

        currTokenIndex++;

        if (currTokenIndex >= tokens.size()) { return false; }

        if (tokens[currTokenIndex].type != ID && tokens[currTokenIndex].type != NUMBER) {
            return false;
        }

        currTokenIndex++;

    }

    return true;

}

void parseProgram(const std::vector<Token> tokens) {

    size_t currTokenIndex = 0;

    for (; currTokenIndex < tokens.size(); currTokenIndex++) {
        parseStatement(tokens,currTokenIndex);
    }
    

}

int main() {
    std::string tokenInput;

    while (true) {
        std::cout << "Enter token stream (space-separated tokens), or 0 to exit:\n";
        std::getline(std::cin, tokenInput);

        if (tokenInput == "0") break;

        // Clear previous errors if you store them globally
        errors.clear();

        // Call the parser
        parseProgram(tokenize(tokenInput));

        // Display errors, if any
        for (const auto& error : errors) {
            std::cout << error << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}