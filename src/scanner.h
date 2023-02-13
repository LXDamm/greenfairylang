#ifndef cyberkvasar_scanner_h
#define cyberkvasar_scanner_h

typedef enum {
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN, TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
    TOKEN_SLASH, TOKEN_STAR, TOKEN_COLON,
    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
    TOKEN_DO, TOKEN_END,
    TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE, TOKEN_CASE, TOKEN_BREAK,
    TOKEN_RET, TOKEN_DEF,
    TOKEN_NULL, TOKEN_TRUE, TOKEN_FALSE,
    TOKEN_EQUAL, TOKEN_BANG, TOKEN_BANG_EQUAL,
    TOKEN_LESS, TOKEN_LESS_EQUAL,
    TOKEN_GREATER, TOKEN_GREATER_EQUAL,
    TOKEN_ECHO, TOKEN_ERROR, TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;

void initScanner(const char* source);
Token scanToken();

#endif