#ifndef TOKENIZER_H
#define TOKENIZER_H

enum token_type {
  TOKEN_WORD,
  TOKEN_PIPE,
  TOKEN_REDIR_IN,
  TOKEN_REDIR_OUT,
  TOKEN_REDIR_APPEND
};

struct token {
  enum token_type type;
  char value[64];
};

int tokenizer(char *input, struct token *tokens);

#endif // TOKENIZER_H
