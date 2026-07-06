#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>

typedef enum {
  TOKEN_COMMAND,
  TOKEN_ARGUMENTS,
  TOKEN_SINGLE_QUOTE,
  TOKEN_DOUBLE_QUOTE,
  TOKEN_EOF
} token_type;

typedef struct {
  const char *start;
  const char *current;
  int pos;
} scanner;

typedef struct {
  token_type type;
  const char *start;
  int length;
  int pos;
} token;

void args_scanner(char *input);
token args_token_scan(scanner *s);
#endif
