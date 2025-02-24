#include "s21_cat.h"

int main(int argc, char **argv) {
  FLAGS flags = {0};
  FILE *file;
  parse_data(argc, argv, &flags);
  // print_struct(&flags);
  for (; optind < argc; optind++) {
    file = fopen(argv[optind], "r");
    print_v_file(file, &flags, argv);
  }
  return 0;
}

void parse_data(int argc, char **argv, FLAGS *flags) {
  struct option longOptions[] = {{"number-nonblank", 0, NULL, 'b'},
                                 {"number", 0, NULL, 'n'},
                                 {"squeeze-blank", 0, NULL, 's'},
                                 {NULL, 0, NULL, 0}};
  int currentFlag = getopt_long(argc, argv, "+benstvTE", longOptions, NULL);
  // при каждом вызове функции  getopt_long()
  // значение глобальной переменной optind увеличивается на 1,
  // Нумерация optind начинается с 1, в конце эта переменная
  // равна количеству флагов + 1
  for (; currentFlag != -1;
       currentFlag = getopt_long(argc, argv, "benstvTE", longOptions, NULL)) {
    set_flags(currentFlag, flags);
  }
}

void set_flags(int currentFlag, FLAGS *flags) {
  switch (currentFlag) {
    case 'b':
      if (flags->n) flags->n = 0;
      flags->b = 1;
      break;
    case 'e':
      flags->e = 1;
      flags->v = 1;
      break;
    case 'n':
      if (!(flags->b)) flags->n = 1;
      break;
    case 's':
      flags->s = 1;
      break;
    case 't':
      flags->t = 1;
      flags->v = 1;
      break;
    case 'v':
      flags->v = 1;
      break;
    case 'T':
      flags->t = 1;
      break;
    case 'E':
      flags->e = 1;
      break;
  };
}

void print_v_file(FILE *file, FLAGS *flags, char **argv) {
  if (file == NULL) {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv[optind]);
    return;
  }
  int flag_nl = 1;
  int flag_s = 0;
  int count_s = 0;
  int line_number = 1;
  for (int symb = getc(file); symb != EOF; symb = getc(file)) {
    // Этот участок кода отвечает за нумерацию строк, непустых для -b и всех для
    // -n
    if (flags->n && flag_nl && NL_SYMB(symb) &&
        count_s < 1)  // этот случай учитывает нумерацию пустых строчек для -n
      // printf("%6dEMPTY(number %d, flag_s %d, flag_nl %d)\t", line_number++,
      // count_s, flag_s, flag_nl);
      printf("%6d\t", line_number++);
    if (NL_SYMB(symb)) {
      if (flags->s) {
        if (flag_nl) {
          flag_s = 1;
          count_s++;
        } else {
          flag_s = 0;
          count_s = 0;
        }
      }
      if (flags->b && flags->e && flag_nl &&
          count_s <= 1)  // этот случай сдвигает \n на уровень остальных
                         // символов при -b
        printf("      \t");
      flag_nl = 1;  // встретили \n, значит готовим флаг(отвечает на вопрос: был
                    // ли прошлый символ \n) для нумерации следующей строки
    } else if (!NL_SYMB(symb) && flag_nl) {  // встретили обычный символ(не \n),
                                             // видим флаг, нумеруем
      flag_nl = 0;
      if (flags->n ||
          flags->b)  // это должно находиться тут для работы флага -s
        printf("%6d\t", line_number++);
    }

    // Этот участок кода отвечает за работу с символами -v, -t, -e
    if (flags->v && NON_READABLE_SYMB(symb))
      printf("^%c", symb + '@');
    else if (flags->v && DEL_SYMB(symb))
      printf("^?");
    else if (flags->v && NON_READABLE_SYMB_M(symb))
      printf("M-^%c", symb - '@');
    else if (flags->t && TB_SYMB(symb))
      printf("^I");
    else if (flags->e && NL_SYMB(symb) &&
             (flags->s == 0 || NL_SYMB(symb) == 0 || flag_s == 0 ||
              count_s == 1)) {
      // printf("{-s : %d, symb : %d, flag_nl : %d}\n", flags -> s, symb,
      // flag_nl);
      printf("$\n");  // мы не печатаем символ если он является \n при этом до
                      // этого был \n и еще и флаг должен быть -s
    } else if (flags->s == 0 || NL_SYMB(symb) == 0 || flag_s == 0 ||
               count_s <= 1) {
      // printf("{-s : %d, symb : %d, flag_nl : %d}\n", flags -> s, symb,
      // flag_nl);
      printf("%c",
             symb);  // тут выводится символ, если к нему ничего не применилось
    }
    // else
    //      printf("{-s : %d, symb : %d, flag_nl : %d}\n", flags -> s, symb,
    //      flag_nl);
  }
}
