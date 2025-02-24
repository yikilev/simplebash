#include "s21_grep.h"

int main(int argc, char **argv) {
  FLAGS flags = {0};
  FILE *file;
  regex_t regex_patterns;
  int err;
  parse_data(argc, argv, &flags);
  // printf("%s\n", flags.patterns);
  (&flags)->files_quantity = argc - optind;
  if ((&flags)->i) {
    err =
        regcomp(&regex_patterns, (&flags)->patterns, REG_ICASE | REG_EXTENDED);
  } else {
    err = regcomp(&regex_patterns, (&flags)->patterns, 0 | REG_EXTENDED);
  }
  if (err)
    perror("error");
  else {
    for (; optind < argc; optind++) {
      // printf("FILE is -> %s\n", argv[optind]);
      file = fopen(argv[optind], "r");
      get_lines(file, regex_patterns, &flags, argv);
    }
    regfree(&regex_patterns);
  }
  free((&flags)->patterns);
  return 0;
}

void parse_data(int argc, char **argv, FLAGS *flags) {
  int currentFlag = getopt(argc, argv, "e:ivclnhsf:o");
  for (; currentFlag != -1; currentFlag = getopt(argc, argv, "e:ivclnhsf:o")) {
    set_flags(currentFlag, flags);
  }
  if (flags->len_pattern == 0) add_pattern(flags, argv[optind++]);
}

void print_struct(FLAGS *flags) {
  printf("flags->e is %d\n", flags->e);
  printf("flags->i is %d\n", flags->i);
  printf("flags->v is %d\n", flags->v);
  printf("flags->c is %d\n", flags->c);
  printf("flags->l is %d\n", flags->l);
  printf("flags->n is %d\n", flags->n);
  printf("flags->h is %d\n", flags->h);
  printf("flags->s is %d\n", flags->s);
  printf("flags->f is %d\n", flags->f);
  printf("flags->o is %d\n", flags->o);
}

void set_flags(int currentFlag, FLAGS *flags) {
  switch (currentFlag) {
    case 'e':
      // if (flags->n) flags->n = 0;
      flags->e = 1;
      add_pattern(flags, optarg);
      break;
    case 'i':
      flags->i = 1;
      break;
    case 'v':
      flags->v = 1;
      break;
    case 'c':
      flags->c = 1;
      break;
    case 'l':
      flags->l = 1;
      break;
    case 'n':
      flags->n = 1;
      break;
    case 'h':
      flags->h = 1;
      break;
    case 's':
      flags->s = 1;
      break;
    case 'f':
      flags->f = 1;
      add_patterns_from_file(flags, optarg);
      break;
    case 'o':
      flags->o = 1;
      break;
  };
}

void get_lines(FILE *file, regex_t regex_patterns, FLAGS *flags, char **argv) {
  char *line = NULL;
  size_t len_line = 0;
  ssize_t result = getline(&line, &len_line, file);
  int line_number = 0;
  int matching = 0;
  char *file_path = argv[optind];
  int file_does_not_exist = 0;
  if (file == NULL) {
    if (!flags->s) perror(file_path);
    file_does_not_exist = 1;
  }
  while (!file_does_not_exist && result != -1) {
    if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
    line_number++;
    matching = 0;
    // printf("%s\n", line);
    regmatch_t match[1];
    if (regexec(&regex_patterns, line, 1, match, 0) == 0) {
      matching = 1;
      char current_match[(int)(match[0].rm_eo - match[0].rm_so + 1)];
      // char *current_match = NULL;
      get_current_match(match, line, current_match);
      if (flags->o) strcpy(line, current_match);
    }
    // printf("matching is -> %d\n", matching);
    if (!file_does_not_exist &&
        ((matching && !flags->v) || (!matching && flags->v))) {
      if (flags->c) {
        flags->count_lines++;
      } else if (!flags->l) {
        if (flags->n && flags->files_quantity > 1 && !flags->h) {
          printf("%s:%d:%s\n", file_path, line_number, line);
        } else if (flags->n) {
          printf("%d:%s\n", line_number, line);
        } else if (flags->files_quantity > 1 && !flags->h) {
          printf("%s:%s\n", file_path, line);
        } else {
          printf("%s\n", line);
        }
      } else if (flags->l) {
        break;
      }
    }
    result = getline(&line, &len_line, file);
  }
  if (flags->c && flags->files_quantity > 1 && !flags->l && !flags->h) {
    printf("%s:%d\n", file_path, flags->count_lines);
  } else if ((flags->c && !flags->l) || (flags->c && flags->h)) {
    printf("%d\n", flags->count_lines);
  } else if (flags->c && flags->l && flags->files_quantity == 1 && !flags->h) {
    printf("%d\n", flags->count_lines ? 1 : 0);
    if (flags->count_lines) printf("%s\n", file_path);
  } else if (flags->c && flags->l && flags->files_quantity > 1 && !flags->h) {
    printf("%s:%d\n", file_path, flags->count_lines ? 1 : 0);
    if (flags->count_lines) printf("%s\n", file_path);
  } else if (flags->l && ((matching && !flags->v) || (!matching && flags->v))) {
    printf("%s\n", file_path);
  }
  flags->count_lines = 0;
  if (file) fclose(file);
  if (line) free(line);
}

void add_pattern(FLAGS *flags, char *pattern) {
  int n = strlen(pattern) * sizeof(char) + 5;
  if (flags->len_pattern == 0) {
    flags->patterns = malloc(1024 * sizeof(char));
    flags->mem_pattern = 1024;
  }
  if (flags->mem_pattern < flags->len_pattern + n) {
    flags->patterns = realloc(flags->patterns, flags->mem_pattern * 2);
    flags->mem_pattern *= 2;
  }
  flags->len_pattern += sprintf(flags->patterns + flags->len_pattern, "%s(%s)",
                                flags->len_pattern ? "|" : "", pattern);
  flags->len_pattern = strlen(flags->patterns);
}

void get_current_match(regmatch_t *match, char *line, char *current_match) {
  int current_match_len = (int)(match[0].rm_eo - match[0].rm_so);
  // printf("START is -> %lld  ", match[0].rm_so);
  // printf("END is -> %lld\n", match[0].rm_eo);
  // printf("LENGTH -> %d\n", current_match_len);
  strncpy(current_match, line + match[0].rm_so, current_match_len);
  current_match[current_match_len] = '\0';
  // printf("CURRENT MATCH -> %s\n", current_match);
}

void add_patterns_from_file(FLAGS *flags, char *file_path) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    if (!flags->s) perror(file_path);
    exit(EXIT_FAILURE);
  }
  char *line;
  size_t len_line = 0;
  ssize_t result = getline(&line, &len_line, file);
  while (result != -1) {
    if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
    add_pattern(flags, line);
    result = getline(&line, &len_line, file);
  }
  if (line) free(line);
  if (file) fclose(file);
}