#ifndef _FILES_H
#define _FILES_H

#include <lib/types.h>
#include <lib/lib.h>
#include <kernel/syscall.h>

#define BOOT_BLOCK_SIZE 64
#define DENTRY_SIZE 64
#define BLOCK_SIZE 0x1000
#define MAX_FILENAME_LENGTH 31

/* filesystem read functions */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t * dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t * dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t * buf, uint32_t length);

/* function used for syscalls */
int32_t copy_file_to_addr(uint8_t* fname, uint32_t addr);
inode_t* get_inode(uint32_t inode_num);

/* filesystem functions */
void fs_init(uint32_t addrs);
int32_t fs_write(int32_t fd, const uint8_t * buf, int32_t nbytes);
int32_t fs_read(file_array * fd, uint8_t * buf, int32_t nbytes);
int32_t fs_open(const uint8_t* filename);
int32_t fs_close(void);
char * autocomplete_command(char * curr_text);

/* directory functions */
int32_t dir_write(file_array* fd, const int8_t* buf, int32_t nbytes);
int32_t dir_read(file_array* fd, int8_t * buf, int32_t length);
int32_t dir_open(const uint8_t* filename);
int32_t dir_close(void);

#endif
