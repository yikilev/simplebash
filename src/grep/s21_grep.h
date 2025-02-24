#ifndef S21_GREP_H
#define S21_GREP_H

#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int count_lines;
  int files_quantity;
  char *patterns;
  int len_pattern;
  int mem_pattern;
} FLAGS;
void parse_data(int argc, char **argv, FLAGS *flags);
// void print_v_file(FILE *file, FLAGS *flags);
void set_flags(int currentFlag, FLAGS *flags);
void print_struct(FLAGS *flags);
void get_lines(FILE *file, regex_t regex_patterns, FLAGS *flags, char **argv);
void add_pattern(FLAGS *flags, char *pattern);
void add_patterns_from_file(FLAGS *flags, char *file_path);
void get_current_match(regmatch_t *match, char *line, char *current_match);
#endif