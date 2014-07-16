#include <elf.h>
#include <stdio.h>
#include <string.h>

#include "compiler.h"

void gen_elf64 (AST * ast, FILE * out)
{
    std::vector<unsigned char> code;
    compile (ast, code);
    compile_exit (code);

    const uint64_t code_offs  = sizeof (Elf64_Ehdr) + sizeof (Elf64_Phdr);
    const uint64_t load_addr  = get_mmap_min_addr () + code_offs;
    const uint64_t total_size = code_offs + code.size ();

    // elf64 elf header
    Elf64_Ehdr ehdr;
    ehdr.e_ident[EI_MAG0]       = ELFMAG0;        // magic (7f 45 4c 46)
    ehdr.e_ident[EI_MAG1]       = ELFMAG1;
    ehdr.e_ident[EI_MAG2]       = ELFMAG2;
    ehdr.e_ident[EI_MAG3]       = ELFMAG3;
    ehdr.e_ident[EI_CLASS]      = ELFCLASS64;     // class (64-bit)
    ehdr.e_ident[EI_DATA]       = ELFDATA2LSB;    // 2's complement little-endian
    ehdr.e_ident[EI_VERSION]    = EV_CURRENT;     // version of ELF specification
    ehdr.e_ident[EI_OSABI]      = ELFOSABI_LINUX; // OS ABI (Linux)
    ehdr.e_ident[EI_ABIVERSION] = 0;              // ABI version
    memset (&ehdr.e_ident[EI_PAD], 0, 7);         // reserved
    ehdr.e_type = ET_EXEC;                  // executable file
    ehdr.e_machine = EM_X86_64;             // machine architecture
    ehdr.e_version = EV_CURRENT;            // file version
    ehdr.e_entry = load_addr;               // entrypoint
    ehdr.e_phoff = sizeof (Elf64_Ehdr);     // program header offset
    ehdr.e_shoff = 0;                       // section header offset
    ehdr.e_flags = 0;                       // flags
    ehdr.e_ehsize = sizeof (Elf64_Ehdr);    // elf64 header size
    ehdr.e_phentsize = sizeof (Elf64_Phdr); // program header entry size
    ehdr.e_phnum = 1;                       // program header count
    ehdr.e_shentsize = 0;                   // section header entry size
    ehdr.e_shnum = 0;                       // section header count
    ehdr.e_shstrndx = SHN_UNDEF;            // section header strtab section index

    // elf64 program header
    Elf64_Phdr phdr;
    phdr.p_type = PT_LOAD;      // segment type: loadable
    phdr.p_flags = PF_X | PF_R; // segment permissions: executable and readable
    phdr.p_offset = code_offs;  // offset of segment in file
    phdr.p_vaddr = load_addr;   // virtual address of segment in memory
    phdr.p_paddr = 0;           // reserved for systems with physical addressing
    phdr.p_filesz = total_size; // size of file image of segment
    phdr.p_memsz = total_size;  // size of memory image of segment
    phdr.p_align = 0;           // alignment

    fwrite (&ehdr, 1, sizeof (Elf64_Ehdr), out);
    fwrite (&phdr, 1, sizeof (Elf64_Phdr), out);
    for (unsigned int i = 0; i < code.size (); ++i)
        fwrite (&code[i], 1, 1, out);
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

uint64_t get_mmap_min_addr ()
{
    uint64_t mmap_min_addr;
    FILE * f = fopen ("/proc/sys/vm/mmap_min_addr", "r");
    fscanf (f, "%lu", &mmap_min_addr);
    fclose (f);
    return mmap_min_addr;
}
