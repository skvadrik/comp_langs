#include "interpreter.h"

int interpret (Node * node)
{
    switch (node->type)
    {
        case Node::OP:
        {
            int left = interpret (node->value.op->left);
            int right = interpret (node->value.op->right);
            switch (node->value.op->type)
            {
                case Op::ADD:
                    return left + right;
                case Op::SUB:
                    return left - right;
                case Op::MUL:
                    return left * right;
                case Op::DIV:
                    return left / right;
            }
        }
        case Node::NUMBER:
            return node->value.number;
    }
}
