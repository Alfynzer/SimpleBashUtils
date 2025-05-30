#include "s21_grep.h"

int main(int arg_count, char *arg_values[]) {
  if (arg_count < 2) {
    fprintf(stderr, "Usage: %s <filename> [filename2] ...\n", arg_values[0]);
    return 1;
  }
  handle_flags(arg_count, arg_values);
  return 0;
}

void set_flag(const char *arg, bool *is_flag_e, bool *is_flag_i,
              bool *is_flag_v, bool *is_flag_c, bool *is_flag_l,
              bool *is_flag_n, bool *is_flag_h) {
  if (strcmp(arg, "-e") == 0) {
    *is_flag_e = true;
  }
  if (strcmp(arg, "-i") == 0) {
    *is_flag_i = true;
  }
  if (strcmp(arg, "-v") == 0) {
    *is_flag_v = true;
  }
  if (strcmp(arg, "-c") == 0) {
    *is_flag_c = true;
  }
  if (strcmp(arg, "-l") == 0) {
    *is_flag_l = true;
  }
  if (strcmp(arg, "-n") == 0) {
    *is_flag_n = true;
  }
  if (strcmp(arg, "-h") == 0) {
    *is_flag_h = true;
  }
}

bool contains(const char *str, const char *pattern, bool ignore_case) {
  if (ignore_case) {
    char *lower_str = strdup(str);
    char *lower_pattern = strdup(pattern);
    if (!lower_str || !lower_pattern) {
      perror("Memory allocation error");
      exit(EXIT_FAILURE);
    }
    bool result = strstr(lower_str, lower_pattern) != NULL;
    free(lower_str);
    free(lower_pattern);
    return result;
  }
  return strstr(str, pattern) != NULL;
}

void handle_flags(int arg_count, char *arg_values[]) {
  bool is_flag_e = true;
  bool is_flag_i = false;
  bool is_flag_v = false;
  bool is_flag_c = false;
  bool is_flag_l = false;
  bool is_flag_n = false;
  bool is_flag_h = false;

  const char *pattern = NULL;
  const char *valid_flags = "eivclnh";

  int count_filename = 0;
  int pattern_index = -1;  // Индекс шаблона

  // Обрабатываем все аргументы
  for (int i = 1; i < arg_count; i++) {
    if (arg_values[i][0] == '-') {  // Обрабатываем флаги
      for (size_t j = 1; j < strlen(arg_values[i]); j++) {
        if (strchr(valid_flags, arg_values[i][j]) == NULL) {
          fprintf(stderr, "grep: illegal option -- %c\n", arg_values[i][j]);
          exit(1);
        }

        // Устанавливаем флаги по символам
        switch (arg_values[i][j]) {
          case 'e':
            is_flag_e = true;
            break;
          case 'i':
            is_flag_i = true;
            break;
          case 'v':
            is_flag_v = true;
            break;
          case 'c':
            is_flag_c = true;
            break;
          case 'l':
            is_flag_l = true;
            break;
          case 'n':
            is_flag_n = true;
            break;
          case 'h':
            is_flag_h = true;
            break;
        }
      }
    } else if (pattern == NULL) {  // Первый аргумент после флагов — это шаблон
      pattern = arg_values[i];
      pattern_index = i;  // Запоминаем индекс шаблона
    } else {  // Остальные аргументы — это файлы
      count_filename++;
    }
  }

  // Если шаблон не был указан
  if (pattern == NULL) {
    fprintf(stderr, "grep: no pattern provided\n");
    exit(1);
  }

  // Теперь обрабатываем файлы (начиная с аргумента, следующего за шаблоном)
  for (int i = pattern_index + 1; i < arg_count; i++) {
    FILE *file = fopen(arg_values[i], "r");
    if (!file) {
      perror("Error opening file");
      continue;
    }
    apply_flags(file, arg_values[i], pattern, count_filename, is_flag_e,
                is_flag_i, is_flag_v, is_flag_c, is_flag_l, is_flag_n,
                is_flag_h);
    fclose(file);
  }
}

void apply_flags(FILE *file, const char *filename, const char *pattern,
                 int count_filename, bool is_flag_e, bool is_flag_i,
                 bool is_flag_v, bool is_flag_c, bool is_flag_l, bool is_flag_n,
                 bool is_flag_h) {
  char string[STRING_BUFFER_SIZE];  // Оригинальная строка
  char processed_S[STRING_BUFFER_SIZE];  // Обработанная строка
  int count_match_S = -1;
  int count_S = 0;
  // char filename_S;

  while (fgets(string, sizeof(string), file) != NULL) {
    char temp_S[STRING_BUFFER_SIZE];  // Временная строка
    strcpy(processed_S, string);

    if (is_flag_i) {
      flag_i(processed_S, temp_S, (char *)pattern);
      strcpy(processed_S, temp_S);
      if (!is_flag_v) {
        is_flag_e = true;
      }
    }
    if ((is_flag_e && !is_flag_v) || (is_flag_l && !is_flag_v) ||
        (is_flag_c && !is_flag_v) || (is_flag_n && !is_flag_v) ||
        (is_flag_h && !is_flag_v)) {
      flag_e(processed_S, temp_S, pattern, is_flag_i, &count_S);
      if (temp_S[0] == '\0') continue;
      strcpy(processed_S, string);
      ++count_match_S;
      if (is_flag_l) {
        printf("%s\n", filename);
        break;
      }
    }
    if (is_flag_v || (is_flag_l && is_flag_v) || (is_flag_c && is_flag_v)) {
      flag_v(processed_S, temp_S, pattern, is_flag_i, &count_S);
      if (temp_S[0] == '\0') continue;
      strcpy(processed_S, string);
      ++count_match_S;
      if (is_flag_l) {
        printf("%s\n", filename);
        break;
      }
    }
    // ВЫВОДИМ
    if (!is_flag_c && !is_flag_l && !is_flag_n && !is_flag_h) {
      if (count_filename == 1) printf("%s", processed_S);
      if (count_filename > 1) printf("%s:%s", filename, processed_S);
    }
    if (is_flag_n && !is_flag_h) {
      if (count_filename == 1) printf("%d:%s", count_S, processed_S);
      if (count_filename > 1)
        printf("%s:%d:%s", filename, count_S, processed_S);
    }
    if (is_flag_h && !is_flag_l && !is_flag_c) {
      if (is_flag_n)
        printf("%d:%s", count_S, processed_S);
      else
        printf("%s", processed_S);
    }
  }
  if (is_flag_c && !is_flag_l) {
    if (count_filename == 1) printf("%d\n", count_match_S + 1);
    if (count_filename > 1)
      printf("%s:%d\n", filename,
             (count_match_S == -1) ? 0 : count_match_S + 1);
  }
}

void flag_e(char *processed_S, char *temp_S, const char *pattern,
            bool ignore_case, int *count_S) {
  if (contains(processed_S, pattern, ignore_case)) {
    strcpy(temp_S, processed_S);
  } else {
    temp_S[0] = '\0';  // Очищаем строку, если она не соответствует
  }
  ++*count_S;  // Флаг -n
}
void flag_i(char *processed_S, char *temp_S, char *pattern) {
  size_t i = 0;

  // Преобразуем строку processed_S в нижний регистр
  while (processed_S[i] != '\0') {
    temp_S[i] = tolower((unsigned char)processed_S[i]);
    i++;
  }
  temp_S[i] = '\0';

  // Преобразуем шаблон в нижний регистр
  if (pattern != NULL) {
    i = 0;
    while (pattern[i] != '\0') {
      pattern[i] = tolower((unsigned char)pattern[i]);
      i++;
    }
  }
}
void flag_v(char *processed_S, char *temp_S, const char *pattern,
            bool ignore_case, int *count_S) {
  if (!contains(processed_S, pattern, ignore_case)) {
    strcpy(temp_S, processed_S);
  } else {
    temp_S[0] = '\0';  // Очищаем строку
  }
  ++*count_S;  // Флаг -n
}

void grep_file() { return; }
