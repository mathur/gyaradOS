#include <fs/files.h>

boot_block b;
uint32_t b_block_addrs;

dentry_t * dentries;
inode_t * inodes;
uint32_t data_blocks;

/**
 * Initializing the Boot block by getting the data from address
 * @param addrs The address of the boot module
 */
void fs_init(uint32_t addrs) {
    b_block_addrs = addrs;
    memcpy(&b, (void *) b_block_addrs, BOOT_BLOCK_SIZE);

    dentries = (dentry_t *)(b_block_addrs + BOOT_BLOCK_SIZE);
    inodes = (inode_t *) (b_block_addrs + BLOCK_SIZE);
    data_blocks = b_block_addrs + b.n_inodes * BLOCK_SIZE + BLOCK_SIZE;
}

/* fs_read(int8_t* fd, uint8_t * buf, int32_t nbytes)
 * inputs: file descriptor fd, output buffer buf, and number of bytes to read
 * nbytes
 * ouputs: return number of bytes read into the buffer for output
 */
int32_t fs_read(file_array* fd, uint8_t * buf, int32_t nbytes) {
    dentry_t temp;

    //checking if file exists
    if(0 != read_dentry_by_name((uint8_t*)(fd->file_name), &temp)) {
        return -1;
    }

    int bytesRead = read_data(temp.inode_num, fd->file_position, buf, nbytes);

    fd->file_position += bytesRead;

    return bytesRead;
}

/* fs_write()
 * inputs: none
 * ouputs: none return -1 because read only
 */
int32_t fs_write(int32_t fd, const uint8_t * buf, int32_t nbytes) {
    return -1;
}

/* fs_open()
 * inputs: none
 * ouputs: none return 0
 */
int32_t fs_open(const uint8_t* filename) {
    return 0;
}

/* fs_close()
 * inputs: none
 * ouputs: none return 0
 */
int32_t fs_close(void) {
    return 0;
}

/**
 * Copies the data of given name to given dentry
 * @param  fname  File name that user wants to copy
 * @param  dentry Where to copy
 * @return        0 if success, -1 if failure
 */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry) {
    int i;

    int readBytes = strlen((int8_t *) fname);

    //error check for out of bounds
    if (readBytes > MAX_FILENAME_LENGTH) {
        readBytes = MAX_FILENAME_LENGTH;
    }

    //loop through dir entries until file name is found
    for(i = 0; i < b.n_dentries; i++) {
        if(strlen((int8_t *) fname) == strlen((int8_t *) dentries[i].file_name)) {
            if(0 == strncmp((int8_t *) dentries[i].file_name, (int8_t *) fname, readBytes)) {
                strncpy((int8_t *) dentry->file_name, (int8_t *) dentries[i].file_name, readBytes);
                dentry->file_type = dentries[i].file_type;
                dentry->inode_num = dentries[i].inode_num;
                return 0;
            }
        }
    }

    //reached end of entries
    return -1;
}

/**
 * Finds and copies the data at given index(if it exists) into given dentry.
 * @param  index  The index of the dentry that user wants to copy
 * @param  dentry Where to copy the information
 * @return        0 if success, -1 if failure
 */
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry) {
    // //check if index out of bounds
    if(index >= b.n_dentries || index < 0)
        return -1;

    //copying the data from the index to the dentry, 32 is max number of bytes for a file name
    strncpy((int8_t *)dentry->file_name, (int8_t *)dentries[index].file_name, 32);
    dentry->file_type = dentries[index].file_type;
    dentry->inode_num = dentries[index].inode_num;

    return 0;
}

/**
 * [read_data  reads the contents of a file]
 * @param  inode  [inode of specified file]
 * @param  offset [current offset within data blocks]
 * @param  buf    [buffer to read to]
 * @param  length [number of bytes to read]
 * @return        [number of bytes read]
 */
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t * buf, uint32_t length) {
    //error checking
    if(inode >= b.n_inodes || inode < 0)
        return -1;

    if(offset >= inodes[inode].file_size)
        return 0;

    int curr_block = offset / BLOCK_SIZE;
    int location_in_block = offset % BLOCK_SIZE;

    uint8_t * curr_read_pos = (uint8_t *) (data_blocks + inodes[inode].blocks[curr_block] * BLOCK_SIZE + location_in_block);

    int num_reads = 0;
    int buf_pos = 0;

    while(num_reads < length) {
        buf[buf_pos] = *curr_read_pos;
        num_reads++;
        buf_pos++;
        curr_read_pos++;
        location_in_block++;

        if(offset + num_reads > inodes[inode].file_size) {
            return num_reads;
        }

        if(location_in_block >= BLOCK_SIZE) {
            // reset location
            location_in_block = 0;
            curr_block++;

            //checking if the data block is invalid
            if(inodes[inode].blocks[curr_block] >= b.n_data_blocks) {
                return -1;
            }

            // recalculate read position
            curr_read_pos = (uint8_t *) (data_blocks + inodes[inode].blocks[curr_block] * BLOCK_SIZE + location_in_block);
        }
    }

    return num_reads;
}

/**
 * Copies a file to a given address
 * @param  fname file name
 * @param  addr  address to copy to
 * @return       0 success, -1 failure
 */
int32_t copy_file_to_addr(uint8_t* fname, uint32_t addr) {
    dentry_t temp_dentry;

    if(0 != read_dentry_by_name(fname, &temp_dentry)) {
        return -1;
    }

    uint32_t file_size = inodes[temp_dentry.inode_num].file_size;

    uint32_t ret = read_data(temp_dentry.inode_num, 0, (uint8_t *) addr, file_size);

    if(ret == -1) {
        return -1;
    }

    return 0;
}

char * autocomplete_command(char * curr_text) {
    int i = 0;
    int str_length = strlen(curr_text);

    for(i = 0; i < b.n_dentries; i++) {
        if(0 == strncmp((int8_t *) dentries[i].file_name, (int8_t *) curr_text, str_length)) {
            return dentries[i].file_name;
        }
    }

    return (char *) -1;
}

/**
 * Returns the inodes from given inode_num
 * @param  inode_num inode number
 * @return           returns the inode of the inode number
 */
inode_t* get_inode(uint32_t inode_num) {
    return inodes + inode_num * BLOCK_SIZE;
}

/* dir_open()
 * inputs: none
 * ouputs: none return 0
 */
int32_t dir_open(const uint8_t* filename){
    return 0;
}

/* dir_close()
 * inputs: none
 * ouputs: none return 0
 */
int32_t dir_close(void){
    return 0;
}

/* dir_write()
 * inputs: none
 * outputs: return -1 because read only
 */
int32_t dir_write(file_array* fd, const int8_t * buf, int32_t nbytes){
    return -1;
}

/* dir_read(int32_t fd, int8_t * buf, int32_t length)
 * description: reads next filename into the buffer eveytime dir_read is called
 * inputs: file descriptor, buffer for output, and length for bytes to copy
 * outputs: copies filename into buf and returns the number of bytes copied
 */
int32_t dir_read(file_array* fd, int8_t * buf, int32_t length){
    //check if reached end
    if(fd->file_position >= b.n_dentries){
        fd->file_position = 0;
        return 0;
    }
    //copy name into buf
    strcpy(buf, dentries[fd->file_position].file_name);

    //get number of bytes and increment the directory read counter
    int bytesCopied = strlen(buf);

    fd->file_position++;

    return bytesCopied;
}
