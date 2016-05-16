#include <mm/paging.h>

/* void init_paging()
 * Description: initializes paging (all pages and page directories)
 * Inputs: none
 * Outputs: none
 */
void init_paging() {
	int i;
    
	/* Disabling the 4kb pages in the first 4MB of the PD by setting the pages to be
     * Supervisor privilege, Read/Write, not present
     */
	for(i = 0; i < PAGE_TABLE_SIZE; i++) {
		pageTable1[i].PTE_bits.page_base = i;
		pageTable1[i].PTE_bits.present = 0;
		pageTable1[i].PTE_bits.read_write = 0;
		pageTable1[i].PTE_bits.user_super = 0;
        pageTable1[i].PTE_bits.write_through = 0;
        pageTable1[i].PTE_bits.cache_disabled = 0;
        pageTable1[i].PTE_bits.accessed = 0;
        pageTable1[i].PTE_bits.dirty = 0;
        pageTable1[i].PTE_bits.page_table_attribute = 0;
        pageTable1[i].PTE_bits.global = 0;
        pageTable1[i].PTE_bits.avail = 0;
	}

    //enabling the present and read/write and user bit for the video memory at physical location 0xB8000
	pageTable1[VIDEO/PAGE_SIZE].PTE_bits.present = 1;
	pageTable1[VIDEO/PAGE_SIZE].PTE_bits.read_write = 1;
	pageTable1[VIDEO/PAGE_SIZE].PTE_bits.user_super = 1; 

    //pages used to store the 3 terminals
    pageTable1[VIDEO_PHYS_ADDR0/PAGE_SIZE].PTE_bits.present = 1;
    pageTable1[VIDEO_PHYS_ADDR0/PAGE_SIZE].PTE_bits.read_write = 1;
    pageTable1[VIDEO_PHYS_ADDR0/PAGE_SIZE].PTE_bits.user_super = 1; 

    pageTable1[VIDEO_PHYS_ADDR1/PAGE_SIZE].PTE_bits.present = 1;
    pageTable1[VIDEO_PHYS_ADDR1/PAGE_SIZE].PTE_bits.read_write = 1;
    pageTable1[VIDEO_PHYS_ADDR1/PAGE_SIZE].PTE_bits.user_super = 1; 

    pageTable1[VIDEO_PHYS_ADDR2/PAGE_SIZE].PTE_bits.present = 1;
    pageTable1[VIDEO_PHYS_ADDR2/PAGE_SIZE].PTE_bits.read_write = 1;
    pageTable1[VIDEO_PHYS_ADDR2/PAGE_SIZE].PTE_bits.user_super = 1; 

    for(i = 0; i < PAGE_DIRECTORY_SIZE; i++){
        //setting the rest of the pages to not be present, read/write, and supervisor privilege
        pageDirectory[i].PDE_bits.page_table_base = 0;
        pageDirectory[i].PDE_bits.present = 0;
        pageDirectory[i].PDE_bits.read_write = 1;
        pageDirectory[i].PDE_bits.user_super = 1;
        pageDirectory[i].PDE_bits.write_through = 0;
        pageDirectory[i].PDE_bits.cache_disabled = 0;
        pageDirectory[i].PDE_bits.accessed = 0;
        pageDirectory[i].PDE_bits.reserved = 0;
        pageDirectory[i].PDE_bits.page_size = 0;
        pageDirectory[i].PDE_bits.global = 0;
        pageDirectory[i].PDE_bits.avail = 0;
    }

    //enabling bits for R/W and Present and supervisor level(0x3)
	pageDirectory[0].PDE_bits.page_table_base = (uint32_t) pageTable1 / _4KB;
	pageDirectory[0].PDE_bits.present = 1;
	pageDirectory[0].PDE_bits.read_write = 1;
    pageDirectory[1].PDE_bits.user_super = 1;
	pageDirectory[0].PDE_bits.page_size = 0;

	//enabling bits PS, R/W, Present (10000011)
	pageDirectory[1].PDE_bits.page_table_base = (uint32_t) KERNEL_PHYS_ADDR / _4KB;
	pageDirectory[1].PDE_bits.page_size = 1;
	pageDirectory[1].PDE_bits.present = 1;
	pageDirectory[1].PDE_bits.read_write = 1;
	pageDirectory[1].PDE_bits.user_super = 0;

    //Enabling paging 
    asm volatile (
                    "mov %%eax, %%cr3 				\n" //moving the address of PD to cr3
                    "mov %%cr4, %%eax 				\n"
                    "or  $0x00000010, %%eax 		\n" //setting PSE in cr4 for 4MB pages
                    "mov %%eax, %%cr4				\n"
                    "mov %%cr0, %%eax				\n" //enabling the PG flag in CR0
                    "or  $0x80000000, %%eax			\n"
                    "mov %%eax, %%cr0"				
                    : /* no outputs */			
                    : "a" (pageDirectory)			
                 );		
}

/* uint32_t init_new_process(uint32_t process_num)
 * Description: maps a new process to the 128MB location
 * Inputs: process_num to allocate memory for a new process
 * Outputs: none
 */

uint32_t init_new_process(uint32_t process_num){
    //get address and index
    uint32_t address = KERNEL_END_ADDR + process_num*PAGE_SIZE_LARGE;
    uint32_t idx = PROCESS_START_ADDR/PAGE_SIZE_LARGE;

    //enable the given page;
    pageDirectory[idx].PDE_bits.page_table_base = address / _4KB;
    pageDirectory[idx].PDE_bits.page_size = 1;
    pageDirectory[idx].PDE_bits.present = 1;
    pageDirectory[idx].PDE_bits.read_write = 1;
    pageDirectory[idx].PDE_bits.user_super = 1;

    //write to cr3
    asm volatile (
                    //moving the address of PD to cr3
                    "mov %%eax, %%cr3               \n" 
                    : /* no outputs */
                    : "a" (pageDirectory)
                 );
    return pageDirectory[idx].PDE_bits.page_table_base;
}

/* int switch_pd(uint8_t process_num, uint32_t prev_base)
 * Description: switches program back to parent program
 * Inputs: process_num and prev_base to switch back to parent process
 * Outputs: none
 */
int switch_pd(uint8_t process_num, uint32_t prev_base) {
    // get address and index
    uint32_t idx = PROCESS_START_ADDR / PAGE_SIZE_LARGE;

    //enable the given page;
    pageDirectory[idx].PDE_bits.page_table_base = prev_base;
    pageDirectory[idx].PDE_bits.page_size = 1;
    pageDirectory[idx].PDE_bits.present = 1;
    pageDirectory[idx].PDE_bits.read_write = 1;
    pageDirectory[idx].PDE_bits.user_super = 1;

    //write to cr3
    asm volatile (
                    //moving the address of PD to cr3
                    "mov %%eax, %%cr3               \n" 
                    : /* no outputs */          
                    : "a" (pageDirectory)           
                 );
    return 0;
}
