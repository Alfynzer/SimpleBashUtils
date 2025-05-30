#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  bool numberNonBlank;
  bool markEndl;
  bool numberAll;
  bool squeeze;
  bool tab;
  bool printNonPrintable;
} Flags;

Flags CatReadFlags(int argc, char *argv[]);
void printFormattedChar(char c, Flags flags);
void printFile(const char *filename, Flags flags);

#endif
