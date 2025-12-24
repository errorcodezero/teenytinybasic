#ifndef INCLUDE_SRC_PARSER_H_
#define INCLUDE_SRC_PARSER_H_

#include "scanner.h"

struct Parser {
  TokensList tokens;
};

// Recursive Descent
void parse(Parser *parser);
void line(Parser *parser);
void number(Parser *parser);
void statement(Parser *parser);
void print_statement(Parser *parser);
void if_statement(Parser *parser);
void relop(Parser *parser);
void gosub(Parser *parser);
void return_statement(Parser *parser);
void clear(Parser *parser);
void list(Parser *parser);
void run(Parser *parser);
void end(Parser *parser);

// Helper functions
Token *advance();

#endif // INCLUDE_SRC_PARSER_H_
