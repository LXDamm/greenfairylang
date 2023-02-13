#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct {
    const char* start;
    const char* current;
    int line;
} Scanner;

Scanner scanner;

void initScanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isAtEnd() {
    return *scanner.current == '\0';
}

static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

static char peek() {
    return *scanner.current;
}

static char peekNext() {
    if (isAtEnd()) return '\0';
    return scanner.current[1];
}

static bool match(char expected) {
    if (isAtEnd()) return false;
    if(*scanner.current != expected) return false;
    scanner.current++;
    return true;
}

/*static bool isDo(char c) {
    if (c != 'd') return false;
    if (!match('o')) return false;
    return true;
}

static bool isEnd(char c) {
    if (c != 'e') return false;
    if (!match('n')) return false;
    if (!match('d')) return false;
    return true;
}*/

static Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;

    return token;
}

static Token errorToken(const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = scanner.line;

    return token;
}

static void skipWhitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t': {
                advance();
                break;
            }
            case '\n': {
                scanner.line++;
                advance();
                break;
            }
            case '/': {
                if (peekNext() == '/') {
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else return;
                break;
            }
            default: return;
        }
    }
}

static TokenType checkKeyword(int start, int length, const char* rest, TokenType type) {
    if (scanner.current - scanner.start == start + length &&
        memcmp(scanner.start + start, rest, length) == 0) {
        return type;
    }
    return TOKEN_IDENTIFIER;
}

static TokenType identifierType() {
    switch (scanner.start[0]) {
        case 'b': return checkKeyword(1, 4, "reak", TOKEN_BREAK);
        case 'c': return checkKeyword(1, 3, "ase", TOKEN_CASE);
        case 'd': {
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'e': return checkKeyword(2, 1, "f", TOKEN_DEF);
                    case 'o': return checkKeyword(2, 0, "o", TOKEN_DO);
                }
            }
            break;
        }
        case 'e': {
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'c': return checkKeyword(2, 2, "ho", TOKEN_ECHO);
                    case 'l': return checkKeyword(2, 2, "se", TOKEN_ELSE);
                    case 'n': return checkKeyword(2, 1, "d", TOKEN_END);
                }
            }
            break;
        }
        case 'f': return checkKeyword(1, 4, "alse", TOKEN_FALSE);
        case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n': return checkKeyword(1, 3, "ull", TOKEN_NULL);
        case 'r': return checkKeyword(1, 2, "et", TOKEN_RET);
        case 't': return checkKeyword(1, 3, "rue", TOKEN_TRUE);
        case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }

    return TOKEN_IDENTIFIER;
}

static Token identifier() {
    while (isAlpha(peek()) || isDigit(peek())) advance();
    return makeToken(identifierType());
}

static Token number() {
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        advance();

        while(isDigit(peek())) advance();
    }

    return makeToken(TOKEN_NUMBER);
}

static Token string() {
    while (peek() != '\'' && !isAtEnd()) {
        if (peek() == '\n') scanner.line++;
        advance();
    }

    if (isAtEnd()) return errorToken("Unterminated string.");

    advance();
    return makeToken(TOKEN_STRING);
}

Token scanToken() {
    skipWhitespace();

    scanner.start = scanner.current;

    if (isAtEnd()) return makeToken(TOKEN_EOF);

    char c = advance();
    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();

    /*if (isDo(c)) return makeToken(TOKEN_DO);
    if (isEnd(c)) return makeToken(TOKEN_END);*/

    switch (c) {
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ':': return makeToken(TOKEN_COLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);
        case '=': return makeToken(TOKEN_EQUAL);
        case '!': return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '<': return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case 39: return string();
    }

    return errorToken("Unexpected character. ");
}