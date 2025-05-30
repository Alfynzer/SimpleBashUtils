CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

all: s21_grep

s21_grep: s21_grep.h s21_grep.c
	$(CC) $(CFLAGS) -o $@ $@.c

clean:
	rm -f s21_grep

rebuild: clean all
