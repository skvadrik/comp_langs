#include <stdio.h>

#include "parser.h"
#include "compiler.h"
#include "interpreter.h"
#include "vm.h"

int main (int argc, char ** argv)
{
    if (argc != 2)
    {
        printf ("usage: calc <string>\n");
        return 1;
    }

    const char * s = argv[1];
    Node * ast = parse_expr (s);

    printf ("i> %d\n", interpret (ast));

    FILE * out = fopen ("code", "w");
    gen_elf64 (ast, out);
    fclose (out);

    std::vector<Insn> bytecode;
    vm_bytecode (ast, bytecode);
    printf ("vm> %d\n", vm_run (bytecode));

    printf ("vm-jit> %d\n", vm_jit (bytecode));

    return 0;
}
