#include <stdio.h>

#include "compiler.h"
#include "interpreter.h"
#include "parser.h"
#include "vm_bytecode.h"
#include "vm_jit.h"
#include "vm_run.h"

int main (int argc, char ** argv)
{
    if (argc != 2)
    {
        printf ("usage: calc <string>\n");
        return 1;
    }

    const char * s = argv[1];
    AST * ast = expr (s);

    printf ("i> %d\n", interpret (ast));

    FILE * out = fopen ("code", "w");
    gen_elf64 (ast, out);
    fclose (out);

    std::vector<Insn> bytecode;
    vm_bytecode (ast, bytecode);
    printf ("vm> %d\n", vm_run (bytecode));

    printf ("vm-jit> %d\n", vm_jit (bytecode));

    delete ast;
    return 0;
}
