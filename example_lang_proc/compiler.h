#include <stdio.h>

#include "code.h"
#include "parser.h"

void gen_elf64 (Node * node, FILE * out);
void compile_exit (Code & code);
void compile (Node * node, Code & code);
