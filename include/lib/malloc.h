#ifndef _MALLOC_H
#define _MALLOC_H

#include <lib/types.h>
#include <kernel/syscall.h>

#define MALLOC_META_SIZE    sizeof(malloc_block)

/* helper functions */
malloc_block * request_space(uint32_t size);
malloc_block * get_free_block(uint32_t size);

/* malloc related functions */
void * malloc(uint32_t size);

#endif /* _MALLOC_H */
