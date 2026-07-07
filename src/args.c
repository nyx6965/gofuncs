#include <stdio.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../include/args.h"

static token args_token_make(scanner *s, token_type type);


static char *helper_strtrim(char *str) {
  char *end;
  while (*str == ' ' || *str == '\t' || *str == '\r') str++;
  if (*str == 0) return str;
  end = str + strlen(str) - 1;
  while (*end == ' ' || *end == '\t' || *end == '\r') end--;
  *(end + 1) = 0;
  return str;
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
static bool helper_ispace(scanner *s) {
  char c = *s->current;
  return c == ' ' || c == '\t' || c == '\r' || c == '\0';
}

static token handle_commands(scanner *s, token_type type) {
  while (*s->current != ' ' && *s->current != '\t' && *s->current != '\r' &&
         *s->current != '\0') {
    s->current++;
  }
  s->pos++;
  if (*s->current == '\0') {
    return args_token_make(s, type);
  }
  s->current++;
  helper_skip_whitespace(s);
  return args_token_make(s, type);
};

static token helper_double(scanner *s) {
  while (*s->current != '"' && *s->current != '\0')
    s->current++;

  s->pos++;
  s->current++;
  helper_skip_whitespace(s);
  return args_token_make(s, TOKEN_DOUBLE_QUOTE);
}

static token helper_single(scanner *s) {
  while (*s->current != '\'' && *s->current != '\0')
    s->current++;

  s->pos++;
  s->current++;
  helper_skip_whitespace(s);
  return args_token_make(s, TOKEN_SINGLE_QUOTE);
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

  if (s->pos == 0) {
    return handle_commands(s, TOKEN_COMMAND);
  }

  switch (c) {
  case '"':
    return helper_double(s);
  case '\'':
    return helper_single(s);
  default:
    return handle_commands(s, TOKEN_ARGUMENTS);
  };
  return args_token_make(s, TOKEN_EOF);
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

    char *m = (char *)malloc(token.length * sizeof(char));
    strncpy(m, token.start, token.length);
    char *trimmed = helper_strtrim(m);
    printf("%2d <<%s>>\n", token.type, trimmed);

    free(m);
    if (token.type == TOKEN_EOF)
      break;
  }
}
