#include "interpreter.h"

int interpret (Node * node)
{
    switch (node->type)
    {
        case NODE_OP:
        {
            int left = interpret (node->value.op->left);
            int right = interpret (node->value.op->right);
            switch (node->value.op->type)
            {
                case OP_ADD:
                    return left + right;
                case OP_SUB:
                    return left - right;
                case OP_MUL:
                    return left * right;
                case OP_DIV:
                    return left / right;
            }
        }
        case NODE_NUMBER:
            return node->value.number;
    }
}
