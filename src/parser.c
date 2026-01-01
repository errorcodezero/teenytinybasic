#include "parser.h"
#include "scanner.h"

bool at_end_parser(Parser *parser) {
  return parser->index >= parser->tokens.length;
}

Token *advance_parser(Parser *parser) {
  return &parser->tokens.list[parser->index++];
}

Token *peek_parser(Parser *parser) {
  return &parser->tokens.list[parser->index];
}

void parse(Parser *parser) {
  while (!at_end_parser(parser)) {
    line(parser);
  }
}

void line(Parser *parser) {
  Token *token = peek_parser(parser);
  while (token->token_type != NEW_LINE || token->token_type != END_OF_FILE) {
    if (token->token_type == NUMBER) {
      // TODO: figure out numbered line for goto magic
      wchar_t *number = token->data;
      advance_parser(parser);
      statement(parser);
    } else {
      statement(parser);
    }
  }
}

void statement(Parser *parser) {
  Token *token = peek_parser(parser);
  if (token->token_type == PRINT) {
    print_statement(parser);
  } else if (token->token_type == GOTO) {
    goto_statement(parser);
  } else if (token->token_type == INPUT) {
    input_statement(parser);
  } else if (token->token_type == LET) {
    let_statement(parser);
  } else if (token->token_type == GOSUB) {
    gosub_statement(parser);
  } else if (token->token_type == RETURN) {
    return_statement(parser);
  } else if (token->token_type == CLEAR) {
    clear_statement(parser);
  } else if (token->token_type == LIST) {
    list_statement(parser);
  } else if (token->token_type == RUN) {
    run_statement(parser);
  } else if (token->token_type == END) {
    end_statement(parser);
  }
}
