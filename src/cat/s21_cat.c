#include "s21_cat.h"

Flags CatReadFlags(int argc, char *argv[]) {
  struct option longOptions[] = {{"number-nonblank", 0, NULL, 'b'},
                                 {"number", 0, NULL, 'n'},
                                 {"squeeze-blank", 0, NULL, 's'},
                                 {NULL, 0, NULL, 0}};

  int currentFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL);
  Flags flags = {false, false, false, false, false, false};

  while (currentFlag != -1) {
    switch (currentFlag) {
      case 'b':
        flags.numberNonBlank = true;
        break;
      case 'e':
      case 'E':
        flags.markEndl = true;
        break;
      case 'v':
        flags.printNonPrintable = true;
        break;
      case 'n':
        flags.numberAll = true;
        break;
      case 's':
        flags.squeeze = true;
        break;
      case 't':
      case 'T':
        flags.printNonPrintable =
            true;  // Если нужно отображать непечатаемые символы
        flags.tab = true;  // Установите флаг для обработки табуляции
        break;
      default:
        fprintf(stderr, "Usage: cat [options] [file...]\n");
        exit(EXIT_FAILURE);
    }
    currentFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL);
  }

  return flags;
}

void printFormattedChar(char c, Flags flags) {
  unsigned char ch =
      (unsigned char)c;  // Приведение к unsigned char для корректной обработки

  if (ch < 32 && ch != '\n' && ch != '\t' &&
      flags.printNonPrintable) {  // Управляющие символы 0-31
    putchar('^');
    putchar(ch + '@');  // Преобразуем в ^A-^Z
  } else if (ch == 127 && flags.printNonPrintable) {  // Символ 127
    putchar('^');
    putchar('?');
  } else if (ch >= 128) {  // Символы 128 и выше
    putchar('M');
    putchar('-');
    if (ch < 128 + 32) {  // Символы M-^A-M-^Z
      putchar('^');
      putchar((ch - 128) + '@');
    } else {
      putchar(ch - 128);  // Печатаемые символы
    }
  } else if (ch == '\t' && flags.tab) {
    printf("^I");
  } else if (ch == '\n' && flags.markEndl) {
    printf("$\n");
  } else {  // Обычные символы
    putchar(c);
  }
}

void printFile(const char *filename, Flags flags) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror(filename);
    return;
  }

  char line[1024];
  int lineNumber = 1;
  int blankLineCount = 0;

  while (fgets(line, sizeof(line), file)) {
    bool isBlankLine = (line[0] == '\n');

    // Обработка флага squeeze
    if (flags.squeeze && isBlankLine) {
      blankLineCount++;
      if (blankLineCount > 1)
        continue;  // Пропускаем повторяющиеся пустые строки
    } else {
      blankLineCount = 0;  // Сбрасываем счетчик при ненулевой строке
    }

    // Обработка номеров строк
    if ((flags.numberNonBlank && !isBlankLine) || (flags.numberAll)) {
      printf("%6d\t", lineNumber++);
    }

    // Печать строки с обработкой специальных символов
    for (size_t i = 0; i < strlen(line); i++) {
      printFormattedChar(line[i], flags);
    }
  }

  fclose(file);
}

int main(int argc, char *argv[]) {
  Flags flags = CatReadFlags(argc, argv);
  if (optind == argc) {  // Если не указаны файлы
    fprintf(stderr, "Usage: cat [options] [file...]\n");
    return EXIT_FAILURE;
  }

  for (int i = optind; i < argc; i++) {
    printFile(argv[i], flags);
  }

  return 0;
}