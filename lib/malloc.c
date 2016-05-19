#include <lib/malloc.h>

malloc_block * head = NULL;
malloc_block * tail = NULL;

malloc_block * request_space(uint32_t size) {
    malloc_block * new_block = sbrk(0);
    void * request = sbrk(size + MALLOC_META_SIZE);
    if(request == (void *) -1) {
        return NULL;
    }

    new_block->size = size;
    new_block->next = NULL;
    new_block->free = 0;

    if(head == NULL) {
        head = new_block;
    }

    if(tail != NULL) {
        tail->next = (struct malloc_block *) new_block;
    }

    tail = new_block;
    return new_block;
}

malloc_block * get_free_block(uint32_t size) {
    malloc_block * curr_block = head;

    while(curr_block != NULL) {
        if(curr_block->free && (curr_block->size >= size)) {
            return curr_block;
        }

        curr_block = (struct malloc_block *) curr_block->next;
    }

    if(curr_block == NULL) {
        curr_block = request_space(size);
    }

    return curr_block;
}

malloc_block * get_block_pointer(uint32_t ptr) {
    return (malloc_block *) ptr - 1;
}

void * malloc(uint32_t size) {
    if(size <= 0) {
        return NULL;
    }

    malloc_block * block = get_free_block(size);
    if(block == NULL) {
        return NULL;
    }

    block->free = 0;

    return (block + 1);
}

void * realloc(void * ptr, uint32_t size) {
    if(ptr == NULL) {
        return malloc(size);
    }

    malloc_block * block = get_block_pointer(ptr);

    if(block->size >= size) {
        return ptr;
    }

    void * new_ptr = malloc(size);
    memcpy(new_ptr, ptr, block->size);
    free(ptr);
    return new_ptr;
}

void * calloc(uint32_t nelem, uint32_t elsize) {
    uint32_t size = nelem * elsize;
    void * ptr = malloc(size);
    memset(ptr, 0, size);
    return ptr;
}

void free(uint32_t ptr) {
    if(ptr == NULL) {
        return;
    }

    malloc_block * block = get_block_pointer(ptr);
    block->free = 1;
}