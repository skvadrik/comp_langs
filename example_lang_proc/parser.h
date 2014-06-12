#ifndef __PARSER__
#define __PARSER__

#include "ast.h"

AST * expr (const char * & s);
AST * expr1 (const char * & s, AST * l);
AST * term (const char * & s);
AST * term1 (const char * & s, AST * l);
AST * factor (const char * & s);
unsigned int number (const char * & s, unsigned int n);
unsigned char digit (const char * & s);

#endif // __PARSER__
