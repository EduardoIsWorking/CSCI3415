/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>
#include <ctype.h>

/* Character classes */
#define LETTER 0
#define DIGIT  1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT     10
#define IDENT       11
#define ASSIGN_OP   20
#define ADD_OP      21
#define SUB_OP      22
#define MULT_OP     23
#define DIV_OP      24
#define LEFT_PAREN  25
#define RIGHT_PAREN 26

/* Globals */
int charClass;
char lexeme[100];
int  nextChar;       /* must be int to hold EOF */
int lexLen;
int token;
int nextToken;
FILE *in_fp;

/* Prototypes (modern style) */
void addChar(void);
void getChar(void);
void getNonBlank(void);
int  lex(void);
int  lookup(char ch);

/******************************************************/
/* main driver */
int main(void) {
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("front.in", "r")) == NULL) {
        printf("ERROR - cannot open front.in \n");
        return 1;
    } else {
        getChar();
        do {
            lex();
        } while (nextToken != EOF);
    }
    return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the token */
int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        default:
            addChar();
            nextToken = EOF;
            break;
    }
    return nextToken;
}

/*****************************************************/
/* addChar - add nextChar to lexeme */
void addChar(void) {
    if (lexLen <= 98) {
        lexeme[lexLen++] = (char)nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - get next character and set its class */
void getChar(void) {
    nextChar = getc(in_fp);
    if (nextChar != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - skip whitespace */
void getNonBlank(void) {
    while (isspace(nextChar))
        getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex(void) {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        /* identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;

        /* integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        /* parentheses and operators */
        case UNKNOWN:
            lookup((char)nextChar);
            getChar();
            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E'; lexeme[1] = 'O'; lexeme[2] = 'F'; lexeme[3] = 0;
            break;
    }

    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
}