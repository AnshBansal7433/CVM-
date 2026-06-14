#include "vm.h"
#include <iostream>

InterpretResult VM::run()
{
    for (;;)
    {
        uint8_t instruction = readByte();

        switch (instruction)
        {
        case OP_CONSTANT:
        {
            push(readConstant());
            break;
        }

        case OP_TRUE:
            push(BOOL_VAL(true));
            break;

        case OP_FALSE:
            push(BOOL_VAL(false));
            break;

        case OP_POP:
            pop();
            break;

        case OP_ADD:
        {
            double b = AS_NUMBER(pop());
            double a = AS_NUMBER(pop());
            push(NUMBER_VAL(a + b));
            break;
        }

        case OP_SUB:
        {
            double b = AS_NUMBER(pop());
            double a = AS_NUMBER(pop());
            push(NUMBER_VAL(a - b));
            break;
        }

        case OP_MUL:
        {
            double b = AS_NUMBER(pop());
            double a = AS_NUMBER(pop());
            push(NUMBER_VAL(a * b));
            break;
        }

        case OP_DIV:
        {
            double b = AS_NUMBER(pop());
            double a = AS_NUMBER(pop());
            push(NUMBER_VAL(a / b));
            break;
        }

        case OP_EQUAL:
        {
            Value b = pop();
            Value a = pop();
            bool result;
            if (IS_NUMBER(a) && IS_NUMBER(b))
                result = AS_NUMBER(a) == AS_NUMBER(b);
            else
                result = AS_BOOL(a) == AS_BOOL(b);
            push(BOOL_VAL(result));
            break;
        }

        case OP_NOT_EQUAL:
        {
            Value b = pop();
            Value a = pop();
            bool result;
            if (IS_NUMBER(a) && IS_NUMBER(b))
                result = AS_NUMBER(a) != AS_NUMBER(b);
            else
                result = AS_BOOL(a) != AS_BOOL(b);
            push(BOOL_VAL(result));
            break;
        }

        case OP_LESS:
        {
            double b = AS_NUMBER(pop());
            double a = AS_NUMBER(pop());
            push(BOOL_VAL(a < b));
            break;
        }

        case OP_LESS_EQUAL:
        {
            double b = AS_NUMBER(pop());
            double a = AS_NUMBER(pop());
            push(BOOL_VAL(a <= b));
            break;
        }

        case OP_GREATER:
        {
            double b = AS_NUMBER(pop());
            double a = AS_NUMBER(pop());
            push(BOOL_VAL(a > b));
            break;
        }

        case OP_GREATER_EQUAL:
        {
            double b = AS_NUMBER(pop());
            double a = AS_NUMBER(pop());
            push(BOOL_VAL(a >= b));
            break;
        }

        case OP_AND:
        {
            Value b = pop();
            Value a = pop();
            push(BOOL_VAL(!isFalsey(a) && !isFalsey(b)));
            break;
        }

        case OP_OR:
        {
            Value b = pop();
            Value a = pop();
            push(BOOL_VAL(!isFalsey(a) || !isFalsey(b)));
            break;
        }

        case OP_NEGATE:
        {
            push(NUMBER_VAL(-AS_NUMBER(pop())));
            break;
        }

        case OP_NOT:
        {
            push(BOOL_VAL(isFalsey(pop())));
            break;
        }

        case OP_LOAD:
        {
            uint8_t slot = readByte();
            ensureGlobalSlot(slot);
            push(globals[slot]);
            break;
        }

        case OP_STORE:
        {
            uint8_t slot = readByte();
            ensureGlobalSlot(slot);
            globals[slot] = pop();
            break;
        }

        case OP_JUMP:
        {
            uint16_t offset = readShort();
            ip += offset;
            break;
        }

        case OP_JUMP_IF_FALSE:
        {
            uint16_t offset = readShort();
            if (isFalsey(peek(0)))
                ip += offset;
            break;
        }

        case OP_LOOP:
        {
            uint16_t offset = readShort();
            ip -= offset;
            break;
        }

        case OP_PRINT:
        {
            printValue(peek(0));
            printf("\n");
            break;
        }

        case OP_SCAN:
        {
            uint8_t slot = readByte();
            ensureGlobalSlot(slot);
            double input;
            std::cin >> input;
            globals[slot] = NUMBER_VAL(input);
            break;
        }

        case OP_RETURN:
            return INTERPRET_OK;
        }
    }
}