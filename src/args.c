#include <ctype.h>
#include <stdio.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../include/args.h"

static token args_token_make(scanner *s, token_type type);

static token handle_commands(scanner *s, token_type type) {
  while ((*s->current != ' ' || *s->current != '\t' || *s->current != '\r') &&
         *s->current != '\0') {
    if ((*s->current == ' ' || *s->current == '\t' || *s->current == '\r') &&
        *s->current == '\0') {
      s->pos++;

      s->current++;
      return args_token_make(s, type);
    }
    s->current++;
  }

  s->current++;
  return args_token_make(s, type);
};

static token helper_double(scanner *s) {
  while (*s->current != '"' && *s->current != '\0') {
    if (*s->current == '\n')
      s->pos++;
    s->current++;
  }

  s->current++;
  return args_token_make(s, TOKEN_DOUBLE_QUOTE);
}
static token helper_single(scanner *s) {
  while (*s->current != '\'' && *s->current != '\0') {
    if (*s->current == '\n')
      s->pos++;
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
  token.pos = s->pos;
  return token;
};

token args_token_scan(scanner *s) {
  s->start = s->current;

  if (*s->current == '\0')
    return args_token_make(s, TOKEN_EOF);

  s->current++;
  char c = s->current[-1];

  // printf("%c", c);

  if (s->pos == 0) {
    return handle_commands(s, TOKEN_COMMAND);
  }else{
     return  handle_commands(s, TOKEN_ARGUMENTS);
  }

  /*
  switch (c) {
  case '"':
    return helper_double(s);
  case '\'':
    return helper_single(s);
  default:
    return ;
  };
*/
 // return args_token_make(s, TOKEN_EOF);
};

void args_scanner(char *input) {

  if (strlen(input) == 0) {
    fprintf(stderr, "No input provided\n");
    return;
  };

  scanner *s = (scanner *)malloc(10 * sizeof(scanner));

  if (s == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1);
  }

  s->start = input;
  s->current = input;

  int pos = -1;
  for (int i = 0; i < strlen(input); i++) {
    token token = args_token_scan(s);

    if (token.pos != pos) {
      printf("%4d ", token.pos);
      pos = token.pos;
    } else {
      printf("   | ");
    }
    printf("%2d '%.*s'\n", token.type, token.length, token.start);

    if (token.type == TOKEN_EOF)
      break;
  }
}
