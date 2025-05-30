CC = gcc
CFLAGS =-Wall -Werror -Wextra -std=c11

all: s21_cat

s21_cat: s21_cat.h s21_cat.c
	$(CC) $(CFLAGS) -o $@ $@.c

clean:
	rm -f s21_cat

rebuild: clean all