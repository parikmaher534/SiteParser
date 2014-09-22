#include "../lib/parser.c"
#include "../lib/helpers.c"
#include <stdio.h>
#include <dirent.h>

void loadTests();

char* pathConcater(char* right, char* left);

void TEST_getTags(char* testRoot, char* name);
