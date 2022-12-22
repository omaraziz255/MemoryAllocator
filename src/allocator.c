//
// Created by omar on 22/12/2022.
//

#include <allocator.hpp>
#include <pthread.h>
#include <string.h>
#include <stdio.h>

header_t* head = NULL;
header_t* tail = NULL;

static header_t*
get_free_block(size_t size) {
    header_t* curr = head;
    while(curr) {
        if(curr->s.is_free && curr->s.size >= size)
            return curr;
        curr = curr->s.next;
    }
    return NULL;
}

void* malloc(intptr_t size) {
    void* block;
    intptr_t total_size;
    header_t* header;
    if(!size)
        return NULL;
    pthread_mutex_lock(&global_malloc_lock);
    header = get_free_block(size);
    if(header) {
        header->s.is_free = 0;
        pthread_mutex_unlock(&global_malloc_lock);
        return (void*)(header + 1);
    }
    total_size = (intptr_t)sizeof(header_t) + size;
    block = sbrk(total_size);
    if(block == (void*) -1) {
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }
    header = block;
    header->s.size = size;
    header->s.is_free = 0;
    header->s.next = NULL;
    if(!head)
        head = header;
    if(tail)
        tail->s.next = header;
    tail = header;
    pthread_mutex_unlock(&global_malloc_lock);
    return (void*)(header + 1);
}

void free(void* block) {
    printf("User defined free\n");
    header_t* header, *tmp;
    void* programbreak;

    if(!block)
        return;
    pthread_mutex_lock(&global_malloc_lock);
    header = (header_t*)block - 1;

    programbreak = sbrk(0);
    if((char*)block + header->s.size == programbreak) {
        if(head == tail) {
            head = tail = NULL;
        } else {
            tmp = head;
            while(tmp) {
                if(tmp->s.next == tail) {
                    tmp->s.next = NULL;
                    tail = tmp;
                }
                tmp = tmp->s.next;
            }
        }
        sbrk(0 - (intptr_t)sizeof(header_t) - (intptr_t)header->s.size);
        pthread_mutex_unlock(&global_malloc_lock);
        return;
    }

    header->s.is_free = 1;
    pthread_mutex_unlock(&global_malloc_lock);
}

void* calloc(size_t num, size_t nSize) {
    printf("User defined calloc");
    size_t size;
    void* block;
    if(!num || !nSize)
        return NULL;
    size = num * nSize;
    if(nSize != size/num)       //Check for multiplication overflow
        return NULL;
    block = malloc((intptr_t)size);
    if(!block)
        return NULL;
    memset(block, 0, size);
    return block;
}

void* realloc(void* block, size_t size) {
    printf("User defined realloc\n");
    header_t* header;
    void* ret;
    if(!block || !size)
        return malloc((intptr_t)size);
    header = (header_t*)block - 1;
    if(header->s.size >= size)
        return block;
    ret = malloc((intptr_t)size);
    if(ret) {
        memcpy(ret, block, header->s.size);
        free(block);
    }
    return ret;
}