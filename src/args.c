#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/args.h"
#include "../include/helper.h"

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

static token handle_double_qoutes(scanner *s) {

  while (*s->current != '"' && *s->current != '\0' ){
               if (*(s->current + 1) != '\0'&& *(s->current + 1) == '"') {
                   s->current++;
                   if (*(s->current + 1) != '\0'&& *(s->current + 1) == '"') {
                       s->current++;
                       s->pos = s->pos+2;
                   }if ((*(s->current + 1) != '\0'&& *(s->current + 1) == ' ')) {

                       break;
                   }
               }
       s->current++;
  }


  s->pos++;
  s->current++;
  helper_skip_whitespace(s);
  return args_token_make(s, TOKEN_DOUBLE_QUOTE);
}

static token handle_single_qoutes(scanner *s) {
  while (*s->current != '\'' && *s->current != '\0')
    s->current++;

  s->pos++;
  s->current++;
  if (*s->current == '\'') {
    s->current++;
    while (*s->current != '\'' && *s->current != '\0')
      s->current++;

    s->pos++;
    s->current++;
  }

  helper_skip_whitespace(s);
  return args_token_make(s, TOKEN_SINGLE_QUOTE);
}

token args_token_make(scanner *s, token_type type) {
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
    return handle_double_qoutes(s);
  case '\'':
    return handle_single_qoutes(s);
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

    if (token.type == TOKEN_EOF)
      break;

    char *m = (char *)malloc(token.length * sizeof(char));
    strncpy(m, token.start, token.length);
    char *trimmed = helper_strtrim(m);
    printf("%2d <<%s>>\n", token.type, trimmed);

    free(m);
  }
}
