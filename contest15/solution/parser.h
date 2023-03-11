#include <stdio.h>

int init_parser(FILE *input);
void free_parser(void);
int next_command(Command *c);