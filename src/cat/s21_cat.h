#ifndef S21CAT_H
#define S21CAT_H
#define _GNU_SOURCE
#include <getopt.h>
#include <stdio.h>
#include <string.h>
typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} FLAGS;

#define NON_READABLE_SYMB(symb) \
  ((symb) > -1 && (symb) < 32 && (symb) != '\n' && (symb) != '\t')
#define DEL_SYMB(symb) ((symb) == 127)
#define NON_READABLE_SYMB_M(symb) ((symb) > 127 && (symb) < 160)
#define NL_SYMB(symb) ((symb) == '\n')
#define TB_SYMB(symb) ((symb) == '\t')

void parse_data(int argc, char **argv, FLAGS *flags);
void print_v_file(FILE *file, FLAGS *flags, char **argv);
void set_flags(int currentFlag, FLAGS *flags);
#endif