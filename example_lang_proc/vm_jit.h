#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <vector>

#include "vm_bytecode.h"

int vm_jit (std::vector<Insn> & bytecode);

class ByteArrayEx
{
    unsigned int size_max;
    unsigned int size;
    unsigned char * buffer;

  public:
    ByteArrayEx ()
        : size_max (1024)
        , size (0)
        , buffer ((unsigned char *) mmap (0, size_max, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0))
    { }

    ~ByteArrayEx ()
    {
        munmap (buffer, size_max);
    }

    void save_byte (unsigned char b)
    {
        if (size >= size_max)
        {
            unsigned char * buffer_old = buffer;
            unsigned int size_max_old = size_max;
            size_max *= 2;
            buffer = (unsigned char *) mmap (0, size_max, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            memcpy (buffer, buffer_old, size_max_old);
            munmap (buffer_old, size_max_old);
        }
        buffer[size++] = b;
    }

    int exec ()
    {
        typedef int (* fptr_t) ();
        fptr_t f = (fptr_t) buffer;
        return f ();
    }
};
