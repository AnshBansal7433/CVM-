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
