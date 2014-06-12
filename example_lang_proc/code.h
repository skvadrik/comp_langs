#ifndef __CODE__
#define __CODE__

class Code
{
    unsigned int size_max;
    unsigned int size;
    unsigned char * buffer;

  public:
    Code ();
    ~Code ();
    void save_byte (unsigned char byte);
    int exec ();
};

#endif // __CODE__
