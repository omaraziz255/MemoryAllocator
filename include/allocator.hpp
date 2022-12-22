//
// Created by omar on 22/12/2022.
//

#ifndef MEMORYALLOCATOR_ALLOCATOR_HPP
#define MEMORYALLOCATOR_ALLOCATOR_HPP

#include <stddef.h>
#include <unistd.h>
#include <bits/pthreadtypes.h>

typedef char ALIGN[16];

union header {
    struct {
        size_t size;
        unsigned is_free;
        union header* next;
    } s;
    ALIGN stub;
};

typedef union header header_t;

header_t *head, *tail;
pthread_mutex_t global_malloc_lock;

void* malloc(intptr_t size);

void free(void* block);

void* calloc(size_t num, size_t nSize);

void* realloc(void* block, size_t size);

#endif //MEMORYALLOCATOR_ALLOCATOR_HPP
