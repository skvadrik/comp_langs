#include <stdio.h>
#include <string.h>

#include "parser.h"

class ByteArray
{
    unsigned int size_max;
    unsigned int size;
    unsigned char * buffer;

  public:
    ByteArray ()
        : size_max (1024)
        , size (0)
        , buffer (new unsigned char [size_max])
    { }

    ~ByteArray ()
    {
        delete [] buffer;
    }

    int get_size () const
    {
        return size;
    }

    void save_byte (unsigned char b)
    {
        if (size >= size_max)
        {
            unsigned char * const buffer_old = buffer;
            const unsigned int size_max_old = size_max;
            size_max *= 2;
            buffer = new unsigned char [size_max];
            memcpy (buffer, buffer_old, size_max_old);
            delete [] buffer_old;
        }
        buffer[size++] = b;
    }

    void dump (FILE * f) const
    {
        fwrite (buffer, 1, size, f);
    }
};

void compile (Node * node, ByteArray & code);
void compile_exit (ByteArray & code);
void gen_elf64 (Node * node, FILE * out);
