#include <stdio.h>

#include <stdbool.h>
#include <stdlib.h>

#include "../include/args.h"

static token args_token_make(scanner *s, token_type type);

static token helper_double(scanner *s) {
  while (*s->current != '"' && *s->current != '\0') {
    if (*s->current == '\n')
      s->line++;
    s->current++;
  }

  s->current++;
  return args_token_make(s, TOKEN_DOUBLE_QUOTE);
}
static token helper_single(scanner *s) {
  while (*s->current != '\'' && *s->current != '\0') {
    if (*s->current == '\n')
      s->line++;
    s->current++;
  }

  s->current++;
  return args_token_make(s, TOKEN_SINGLE_QUOTE);
}

static void helper_skip_whitespace(scanner *s) {
  for (;;) {
    char c = *s->current;
    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      s->current++;
      break;
    default:
      return;
    }
  }
}

static token args_token_make(scanner *s, token_type type) {
  token token;
  token.type = type;
  token.start = s->start;
  token.length = (int)(s->current - s->start);
  token.line = s->line;
  return token;
};

token args_token_scan(scanner *s) {
  s->start = s->current;

  if (*s->current == '\0')
    return args_token_make(s, TOKEN_EOF);

  s->current++;
  char c = s->current[-1];

  switch (c) {
  case '"':
    return helper_double(s);
  case '\'':
    return helper_single(s);

  };

  return args_token_make(s, TOKEN_EOF);
};

void args_scanner(char *input) {

  scanner *s = (scanner *)malloc(10 * sizeof(scanner));
  int line = -1;

  for (;;) {
    token token = args_token_scan(s);
    if (token.line != line) {
      printf("%4d ", token.line);
      line = token.line;
    } else {
      printf("   | ");
    }
    printf("%2d '%.*s'\n", token.type, token.length, token.start);

    if (token.type == TOKEN_EOF)
      break;
  }
}
