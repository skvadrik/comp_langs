#include <elf.h>

#include "compiler.h"

void gen_elf64 (Node * node, FILE * out)
{
    Code code;
    compile (node, code);
    compile_exit (code);

    const uint64_t load_addr = 0x400000;
    const uint64_t total_size = sizeof (Elf64_Ehdr) + sizeof (Elf64_Phdr) + code.get_size ();

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
    code.dump (out);
}

void compile_exit (Code & code)
{
    // mov rbx, rax ; 89 c3
    code.save_byte (0x89);
    code.save_byte (0xc3);
    // mov rax, 0x01 ; b8 01 00 00 00 (little-endian)
    code.save_byte (0xb8);
    code.save_byte (0x01);
    code.save_byte (0);
    code.save_byte (0);
    code.save_byte (0);
    // int 80 ; cd 80
    code.save_byte (0xcd);
    code.save_byte (0x80);
}

void compile (Node * node, Code & code)
{
    switch (node->type)
    {
        case NODE_OP:
        {
            compile (node->value.op->left, code);

            // push rax ; 50
            code.save_byte (0x50);

            compile (node->value.op->right, code);

            // mov rbx, rax ; 89 c3
            code.save_byte (0x89);
            code.save_byte (0xc3);
            // pop rax ; 58
            code.save_byte (0x58);

            switch (node->value.op->type)
            {
                case OP_ADD:
                    // add rax, rbx ; 01 d8
                    code.save_byte (0x01);
                    code.save_byte (0xd8);
                    return;
                case OP_SUB:
                    // sub rax, rbx ; 29 d8
                    code.save_byte (0x29);
                    code.save_byte (0xd8);
                    return;
                case OP_MUL:
                    // imul rbx ; f7 eb
                    code.save_byte (0xf7);
                    code.save_byte (0xeb);
                    return;
                case OP_DIV:
                    // xor rdx, rdx ; 31 d2
                    code.save_byte (0x31);
                    code.save_byte (0xd2);
                    // idiv rbx ; f7 fb
                    code.save_byte (0xf7);
                    code.save_byte (0xfb);
                    return;
            }
        }
        case NODE_NUMBER:
        {
            // mov rax, <imm32> ; b8 <byte1> <byte2> <byte3> <byte4> (little-endian)
            unsigned char byte1 = (node->value.number >> (8 * 0)) & 0xFF;
            unsigned char byte2 = (node->value.number >> (8 * 1)) & 0xFF;
            unsigned char byte3 = (node->value.number >> (8 * 2)) & 0xFF;
            unsigned char byte4 = (node->value.number >> (8 * 3)) & 0xFF;
            code.save_byte (0xb8);
            code.save_byte (byte1);
            code.save_byte (byte2);
            code.save_byte (byte3);
            code.save_byte (byte4);
            return;
        }
    }
}
