#include <stdint.h>
#include <stdio.h>
#include <vector>

#include "parser.h"

void compile (AST * ast, std::vector<unsigned char> & code);
void compile_operands (AST * ast, std::vector<unsigned char> & code);
void compile_exit (std::vector<unsigned char> & code);
void gen_elf64 (AST * ast, FILE * out);
uint64_t get_mmap_min_addr ();
