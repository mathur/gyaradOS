#ifndef _MALLOC_H
#define _MALLOC_H

#include <lib/types.h>
#include <kernel/syscall.h>

#define MALLOC_META_SIZE    sizeof(malloc_block)

/* helper functions */
malloc_block * request_space(uint32_t size);
malloc_block * get_free_block(uint32_t size);
malloc_block * get_block_pointer(uint32_t ptr);

/* malloc related functions */
void * malloc(uint32_t size);
void * realloc(void * ptr, uint32_t size);
void * calloc(uint32_t nelem, uint32_t elsize);
void free(uint32_t ptr);

#endif /* _MALLOC_H */
