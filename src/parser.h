#ifndef INCLUDE_SRC_PARSER_H_
#define INCLUDE_SRC_PARSER_H_

#include "scanner.h"

struct Parser {
  TokensList tokens;
  size_t index;
};

// Recursive Descent
void parse(Parser *parser);
void line(Parser *parser);
void number(Parser *parser);
void statement(Parser *parser);
void print_statement(Parser *parser);
void input_statement(Parser *parser);
void if_statement(Parser *parser);
void goto_statement(Parser *parser);
void let_statement(Parser *parser);
void relop(Parser *parser);
void gosub_statement(Parser *parser);
void return_statement(Parser *parser);
void clear_statement(Parser *parser);
void list_statement(Parser *parser);
void run_statement(Parser *parser);
void end_statement(Parser *parser);

// Helper functions
Token *advance_parser(Parser *parser);
Token *peek_parser(Parser *parser);
bool at_end_parser(Parser *parser);

#endif // INCLUDE_SRC_PARSER_H_
