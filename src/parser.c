#include "parser.h"
#include "scanner.h"

bool at_end_parser(Parser *parser) {
  return parser->index >= parser->tokens.length;
}

Token *advance_parser(Parser *parser) {
  return &parser->tokens.list[parser->index++];
}

void parse(Parser *parser) {
  while (!at_end_parser(parser)) {
    line(parser);
  }
}
void line(Parser *parser) {
  Token *token = advance_parser(parser);
  while (token->token_type != NEW_LINE || token->token_type != END_OF_FILE) {
    if (token->token_type == NUMBER) {
      // TODO: figure out numbered line for goto magic
    } else {
      statement(parser);
    }
  }
}
