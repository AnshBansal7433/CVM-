#include <cstdlib>
#include <cstdio>

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)
#define GROW_ARRAY(type, pointer, oldCount, newCount) ((type *)reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount)))
#define FREE_ARRAY(type, pointer, oldCount) (reallocate(pointer, sizeof(type) * (oldCount), 0))

void *reallocate(void *pointer, size_t oldSize, size_t newSize)
{
    if (newSize == 0)
    {
        free(pointer);
        return nullptr;
    }

    void *result = realloc(pointer, newSize);
    return result;
}

typedef enum
{
    VAL_BOOL,
    VAL_NULL,
    VAL_NUMBER,
} ValueType;

typedef struct
{
    ValueType type;
    union
    {
        bool boolean;
        double number;
    } as;
} Value;

#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NIL(value) ((value).type == VAL_NIL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)

#define BOOL_VAL(value) ((Value){VAL_BOOL, {.boolean = value}})
#define NULL_VAL ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})

typedef struct
{
    int capacity;
    int count;
    Value *values;
} ValueArray;

void initValueArray(ValueArray *arr)
{
    arr->capacity = 0;
    arr->count = 0;
    arr->values = NULL;
}

void writeValueArray(ValueArray *arr, Value value)
{
    if (arr->capacity < arr->count + 1)
    {
        int oldCapacity = arr->capacity;
        arr->capacity = GROW_CAPACITY(oldCapacity);
        arr->values = GROW_ARRAY(Value, arr->values, oldCapacity, arr->capacity);
    }
    arr->values[arr->count] = value;
    arr->count++;
}

void freeValueArray(ValueArray *arr)
{
    FREE_ARRAY(Value, arr->values, arr->capacity);
    initValueArray(arr);
}

void printValue(Value value)
{
    printf("%g", AS_NUMBER(value));
}