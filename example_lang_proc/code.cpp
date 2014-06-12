#include <string.h>
#include <sys/mman.h>

#include "code.h"

Code::Code ()
    : size_max (1024)
    , size (0)
    , buffer ((unsigned char *) mmap (0, size_max, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0))
{ }

Code::~Code ()
{
    munmap (buffer, size_max);
}

void Code::save_byte (unsigned char byte)
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
    buffer[size++] = byte;
}

int Code::exec ()
{
    typedef int (* fptr_t) ();
    fptr_t f = (fptr_t) buffer;
    return f ();
}
