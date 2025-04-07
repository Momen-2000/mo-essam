#include <iostream>
#include <fstream>
#include <cctype>
#include <string>

using namespace std;

enum CharClass { LETTER, DIGIT, UNKNOWN, END_OF_FILE };
enum TokenType {
    INT_LIT = 10, IDENT, ASSIGN_OP = 20, ADD_OP, SUB_OP,
    MULT_OP, DIV_OP, LEFT_PAREN, RIGHT_PAREN, END
};

class Lexer {
private:
    CharClass charClass;
    string lexeme;
    char nextChar;
    TokenType nextToken;
    ifstream inputFile;

public:
    Lexer(const string& filename) {
        inputFile.open(filename);
        if (!inputFile) {
            cerr << "ERROR - cannot open " << filename << endl;
            exit(1);
        }
        getChar();
    }

    void getChar() {
        if (inputFile.get(nextChar)) {
            if (isalpha(nextChar))
                charClass = LETTER;
            else if (isdigit(nextChar))
                charClass = DIGIT;
            else
                charClass = UNKNOWN;
        }
        else {
            charClass = END_OF_FILE;
        }
    }

    void getNonBlank() {
        while (isspace(nextChar))
            getChar();
    }

    TokenType lookup(char ch) {
        switch (ch) {
        case '(': lexeme += ch; return LEFT_PAREN;
        case ')': lexeme += ch; return RIGHT_PAREN;
        case '+': lexeme += ch; return ADD_OP;
        case '-': lexeme += ch; return SUB_OP;
        case '*': lexeme += ch; return MULT_OP;
        case '/': lexeme += ch; return DIV_OP;
        default:  lexeme += ch; return END;
        }
    }

    TokenType lex() {
        lexeme.clear();
        getNonBlank();

        switch (charClass) {
        case LETTER:
            lexeme += nextChar;
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                lexeme += nextChar;
                getChar();
            }
            nextToken = IDENT;
            break;

        case DIGIT:
            lexeme += nextChar;
            getChar();
            while (charClass == DIGIT) {
                lexeme += nextChar;
                getChar();
            }
            nextToken = INT_LIT;
            break;

        case UNKNOWN:
            nextToken = lookup(nextChar);
            getChar();
            break;

        case END_OF_FILE:
            lexeme = "EOF";
            nextToken = END;
            break;
        }

        cout << "Next token is: " << nextToken << ", Next lexeme is: " << lexeme << endl;
        return nextToken;
    }

    bool isEOF() const {
        return nextToken == END;
    }
};

int main() {
    Lexer lexer("front.in");

    do {
        lexer.lex();
    } while (!lexer.isEOF());

    return 0;
}