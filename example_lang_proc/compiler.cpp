#include <elf.h>
#include <string.h>

#include "compiler.h"

void compile (AST * ast, std::vector<unsigned char> & code)
{
    switch (ast->type)
    {
        case AST::NUMBER:
        {
            // mov eax, <imm32> ; b8 <byte1> <byte2> <byte3> <byte4> (little-endian)
            unsigned char byte1 = (ast->value.number >> (8 * 0)) & 0xFF;
            unsigned char byte2 = (ast->value.number >> (8 * 1)) & 0xFF;
            unsigned char byte3 = (ast->value.number >> (8 * 2)) & 0xFF;
            unsigned char byte4 = (ast->value.number >> (8 * 3)) & 0xFF;
            code.push_back (0xb8);
            code.push_back (byte1);
            code.push_back (byte2);
            code.push_back (byte3);
            code.push_back (byte4);
            return;
        }
        case AST::ADD:
        {
            compile_operands (ast, code);
            // add eax, ebx ; 01 d8
            code.push_back (0x01);
            code.push_back (0xd8);
            return;
        }
        case AST::SUB:
        {
            compile_operands (ast, code);
            // sub eax, ebx ; 29 d8
            code.push_back (0x29);
            code.push_back (0xd8);
            return;
        }
        case AST::MUL:
        {
            compile_operands (ast, code);
            // imul ebx ; f7 eb
            code.push_back (0xf7);
            code.push_back (0xeb);
            return;
        }
        case AST::DIV:
        {
            compile_operands (ast, code);
            // cdq ; 99, sign-extend edx:eax of eax
            code.push_back (0x99);
            // idiv ebx ; f7 fb
            code.push_back (0xf7);
            code.push_back (0xfb);
            return;
        }
    }
}

void compile_operands (AST * ast, std::vector<unsigned char> & code)
{
    compile (ast->value.operands.left, code);
    // push rax ; 50
    code.push_back (0x50);
    compile (ast->value.operands.right, code);
    // mov ebx, eax ; 89 c3
    code.push_back (0x89);
    code.push_back (0xc3);
    // pop rax ; 58
    code.push_back (0x58);
}

void compile_exit (std::vector<unsigned char> & code)
{
    // mov ebx, eax ; 89 c3
    code.push_back (0x89);
    code.push_back (0xc3);
    // mov eax, 0x01 ; b8 01 00 00 00 (little-endian)
    code.push_back (0xb8);
    code.push_back (0x01);
    code.push_back (0);
    code.push_back (0);
    code.push_back (0);
    // int 80 ; cd 80
    code.push_back (0xcd);
    code.push_back (0x80);
}

void gen_elf64 (AST * ast, FILE * out)
{
    std::vector<unsigned char> code;
    compile (ast, code);
    compile_exit (code);

    const uint64_t load_addr = 0x400000;
    const uint64_t total_size = sizeof (Elf64_Ehdr) + sizeof (Elf64_Phdr) + code.size ();

    // elf64 elf header
    Elf64_Ehdr ehdr;
    ehdr.e_ident[EI_MAG0]       = ELFMAG0;       // magic (7f 45 4c 46)
    ehdr.e_ident[EI_MAG1]       = ELFMAG1;
    ehdr.e_ident[EI_MAG2]       = ELFMAG2;
    ehdr.e_ident[EI_MAG3]       = ELFMAG3;
    ehdr.e_ident[EI_CLASS]      = ELFCLASS64;    // class (64-bit)
    ehdr.e_ident[EI_DATA]       = ELFDATA2LSB;   // 2's complement little-endian
    ehdr.e_ident[EI_VERSION]    = EV_CURRENT;    // version of ELF specification
    ehdr.e_ident[EI_OSABI]      = ELFOSABI_SYSV; // OS ABI (UNIX System V)
    ehdr.e_ident[EI_ABIVERSION] = 0;             // ABI version
    memset (&ehdr.e_ident[EI_PAD], 0, 7);        // reserved
    ehdr.e_type = ET_EXEC;                  // executable file
    ehdr.e_machine = EM_X86_64;             // machine architecture
    ehdr.e_version = EV_CURRENT;            // file version
    ehdr.e_entry
        = load_addr
        + sizeof (Elf64_Ehdr)
        + sizeof (Elf64_Phdr);              // entrypoint
    ehdr.e_phoff = sizeof (Elf64_Ehdr);     // program header offset
    ehdr.e_shoff = 0;                       // section header offset
    ehdr.e_flags = 0;                       // flags
    ehdr.e_ehsize = sizeof (Elf64_Ehdr);    // elf64 header size
    ehdr.e_phentsize = sizeof (Elf64_Phdr); // program header entry size
    ehdr.e_phnum = 1;                       // program header count
    ehdr.e_shentsize = 0;                   // section header entry size
    ehdr.e_shnum = 0;                       // section header count
    ehdr.e_shstrndx = 0;                    // section header strtab section index

    // elf64 program header
    Elf64_Phdr phdr;
    phdr.p_type = PT_LOAD;      // segment type: loadable
    phdr.p_flags = PF_X | PF_R; // segment permissions: executable and readable
    phdr.p_offset = 0;
    phdr.p_vaddr = load_addr;   // virtual address of segment in memory
    phdr.p_paddr = load_addr;   // physical address of segment in memory
    phdr.p_filesz = total_size; // size of file image of segment
    phdr.p_memsz = total_size;  // size of memory image of segment
    phdr.p_align = 0x200000;    // alignment

    fwrite (&ehdr, 1, sizeof (Elf64_Ehdr), out);
    fwrite (&phdr, 1, sizeof (Elf64_Phdr), out);
    for (unsigned int i = 0; i < code.size (); ++i)
        fwrite (&code[i], 1, 1, out);
}
