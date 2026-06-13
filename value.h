#pragma once

#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstddef>

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (static_cast<type*>(reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount))))

#define FREE_ARRAY(type, pointer, oldCount) \
    (reallocate(pointer, sizeof(type) * (oldCount), 0))

inline void* reallocate(void* pointer, size_t oldSize, size_t newSize)
{
    (void)oldSize;

    if (newSize == 0)
    {
        free(pointer);
        return nullptr;
    }

    void* result = realloc(pointer, newSize);

    if (result == nullptr)
    {
        fprintf(stderr, "Out of memory.\n");
        std::exit(1);
    }

    return result;
}

enum ValueType
{
    VAL_BOOL,
    VAL_NULL,
    VAL_NUMBER,
    VAL_STRING
};

struct Value
{
    ValueType type;

    union
    {
        bool boolean;
        double number;
        std::string* str;
    } as;
};

#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NIL(value) ((value).type == VAL_NULL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define IS_STRING(value) ((value).type == VAL_STRING)

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)
#define AS_STRING(value) ((value).as.str)

inline Value BOOL_VAL(bool value)
{
    Value v;
    v.type = VAL_BOOL;
    v.as.boolean = value;
    return v;
}

inline Value NULL_VAL()
{
    Value v;
    v.type = VAL_NULL;
    v.as.number = 0;
    return v;
}

inline Value NUMBER_VAL(double value)
{
    Value v;
    v.type = VAL_NUMBER;
    v.as.number = value;
    return v;
}
inline Value STRING_VAL(std::string* value)
{
    Value v;
    v.type = VAL_STRING;
    v.as.str = value;
    return v;
}

struct ValueArray
{
    int capacity = 0;
    int count = 0;
    Value* values = nullptr;
};

inline void initValueArray(ValueArray* arr)
{
    arr->capacity = 0;
    arr->count = 0;
    arr->values = nullptr;
}

inline void writeValueArray(ValueArray* arr, Value value)
{
    if (arr->capacity < arr->count + 1)
    {
        int oldCapacity = arr->capacity;
        arr->capacity = GROW_CAPACITY(oldCapacity);

        arr->values =
            GROW_ARRAY(Value,
                       arr->values,
                       oldCapacity,
                       arr->capacity);
    }

    arr->values[arr->count++] = value;
}

inline void freeValueArray(ValueArray* arr)
{
    FREE_ARRAY(Value, arr->values, arr->capacity);
    initValueArray(arr);
}

inline void printValue(Value value)
{
    switch (value.type)
    {
        case VAL_BOOL:
            printf("%s", AS_BOOL(value) ? "true" : "false");
            break;

        case VAL_NULL:
            printf("null");
            break;

        case VAL_NUMBER:
            printf("%g", AS_NUMBER(value));
            break;

        case VAL_STRING:
            printf("%s", AS_STRING(value)->c_str());
            break;
    }
}