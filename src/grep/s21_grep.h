#ifndef S21_GREP_H
#define S21_GREP_H

#define _POSIX_C_SOURCE 200809L
#define STRING_BUFFER_SIZE 2048

#include <ctype.h>  // Для функции tolower()
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функция для чтения и вывода файла
void grep_file();

// Функция применения флагов и обработки строк
void apply_flags(FILE *file, const char *filename, const char *pattern,
                 int count_filename, bool is_flag_e, bool is_flag_i,
                 bool is_flag_v, bool is_flag_c, bool is_flag_l, bool is_flag_n,
                 bool is_flag_h);

// Функция установки флагов
void set_flag(const char *arg, bool *is_flag_e, bool *is_flag_i,
              bool *is_flag_v, bool *is_flag_c, bool *is_flag_l,
              bool *is_flag_n, bool *is_flag_h);

// Функция для обработки флагов
void handle_flags(int argc, char *argv[]);

// Функции для обработки отдельных флагов
void flag_e(char *processed_S, char *temp_S, const char *pattern,
            bool ignore_case, int *count_S);
void flag_i(char *processed_S, char *temp_S, char *pattern);
void flag_v(char *processed_S, char *temp_S, const char *pattern,
            bool ignore_case, int *count_S);

// Вспомогательная функция для проверки наличия подстроки
bool contains(const char *str, const char *pattern, bool ignore_case);

#endif  // S21_GREP_H
