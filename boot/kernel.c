/* kernel.c - the C part of the kernel
 * vim:ts=4 noexpandtab
 */

#include <drivers/i8259.h>
#include <drivers/mouse.h>
#include <drivers/pit.h>
#include <drivers/rtc.h>
#include <drivers/speaker.h>
#include <lib/lib.h>
#include <boot/multiboot.h>
#include <boot/x86_desc.h>
#include <lib/debug.h>
#include <mm/paging.h>
#include <fs/files.h>
#include <kernel/syscall.h>

/* Macros. */
/* Check if the bit BIT in FLAGS is set. */
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))
#define DEBUG_F 0

/* Check if MAGIC is valid and print the Multiboot information structure
   pointed by ADDR. */
void entry (unsigned long magic, unsigned long addr) {
	multiboot_info_t *mbi;
	uint32_t fs_start = 0;

	/* Clear the screen. */
	clear();

	/* Am I booted by a Multiboot-compliant boot loader? */
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf ("Invalid magic number: 0x%#x\n", (unsigned) magic);
		return;
	}

	/* Set MBI to the address of the Multiboot information structure. */
	mbi = (multiboot_info_t *) addr;

	/* Print out the flags. */
	if(DEBUG_F) {
		printf ("flags = 0x%#x\n", (unsigned) mbi->flags);
	}

	/* Are mem_* valid? */
	if (CHECK_FLAG (mbi->flags, 0))
		printf ("mem_lower = %uKB, mem_upper = %uKB\n",
				(unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);

	/* Is boot_device valid? */
	if (CHECK_FLAG (mbi->flags, 1))
		printf ("boot_device = 0x%#x\n", (unsigned) mbi->boot_device);

	/* Is the command line passed? */
	if (CHECK_FLAG (mbi->flags, 2))
		printf ("cmdline = %s\n", (char *) mbi->cmdline);

	if (CHECK_FLAG (mbi->flags, 3)) {
		int mod_count = 0;
		int i;
		module_t* mod = (module_t*)mbi->mods_addr;
		while(mod_count < mbi->mods_count) {
			if(mod->string == MOD_FS_STRING) {
				fs_start = (unsigned int) mod->mod_start;
			}

			if(DEBUG_F) {
				printf("Module %d loaded at address: 0x%#x\n", mod_count, (unsigned int)mod->mod_start);
				printf("Module %d ends at address: 0x%#x\n", mod_count, (unsigned int)mod->mod_end);
				printf("First few bytes of module:\n");
				for(i = 0; i<16; i++) {
					printf("0x%x ", *((char*)(mod->mod_start+i)));
				}
				printf("\n");
			}
			mod_count++;
			mod++;
		}
	}
	/* Bits 4 and 5 are mutually exclusive! */
	if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5))
	{
		printf ("Both bits 4 and 5 are set.\n");
		return;
	}

	/* Is the section header table of ELF valid? */
	if (CHECK_FLAG (mbi->flags, 5))
	{
		elf_section_header_table_t *elf_sec = &(mbi->elf_sec);

		printf ("elf_sec: num = %u, size = 0x%#x,"
				" addr = 0x%#x, shndx = 0x%#x\n",
				(unsigned) elf_sec->num, (unsigned) elf_sec->size,
				(unsigned) elf_sec->addr, (unsigned) elf_sec->shndx);
	}

	/* Are mmap_* valid? */
	if (CHECK_FLAG (mbi->flags, 6)) {
		memory_map_t *mmap;

		printf ("mmap_addr = 0x%#x, mmap_length = 0x%x\n",
				(unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
		for (mmap = (memory_map_t *) mbi->mmap_addr;
				(unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
				mmap = (memory_map_t *) ((unsigned long) mmap
					+ mmap->size + sizeof (mmap->size)))
			if(DEBUG_F) {
				printf (" size = 0x%x,     base_addr = 0x%#x%#x\n"
					"     type = 0x%x,  length    = 0x%#x%#x\n",
					(unsigned) mmap->size,
					(unsigned) mmap->base_addr_high,
					(unsigned) mmap->base_addr_low,
					(unsigned) mmap->type,
					(unsigned) mmap->length_high,
					(unsigned) mmap->length_low);
			}
	}

	/* Construct an LDT entry in the GDT */
	{
		seg_desc_t the_ldt_desc;
		the_ldt_desc.granularity    = 0;
		the_ldt_desc.opsize         = 1;
		the_ldt_desc.reserved       = 0;
		the_ldt_desc.avail          = 0;
		the_ldt_desc.present        = 1;
		the_ldt_desc.dpl            = 0x0;
		the_ldt_desc.sys            = 0;
		the_ldt_desc.type           = 0x2;

		SET_LDT_PARAMS(the_ldt_desc, &ldt, ldt_size);
		ldt_desc_ptr = the_ldt_desc;
		lldt(KERNEL_LDT);
	}

	/* Construct a TSS entry in the GDT */
	{
		seg_desc_t the_tss_desc;
		the_tss_desc.granularity    = 0;
		the_tss_desc.opsize         = 0;
		the_tss_desc.reserved       = 0;
		the_tss_desc.avail          = 0;
		the_tss_desc.seg_lim_19_16  = TSS_SIZE & 0x000F0000;
		the_tss_desc.present        = 1;
		the_tss_desc.dpl            = 0x0;
		the_tss_desc.sys            = 0;
		the_tss_desc.type           = 0x9;
		the_tss_desc.seg_lim_15_00  = TSS_SIZE & 0x0000FFFF;

		SET_TSS_PARAMS(the_tss_desc, &tss, tss_size);

		tss_desc_ptr = the_tss_desc;

		tss.ldt_segment_selector = KERNEL_LDT;
		tss.ss0 = KERNEL_DS;
		tss.esp0 = 0x800000;
		ltr(KERNEL_TSS);
	}

	/* Initialize devices, memory, filesystem, enable device interrupts on the
	 * PIC, any other initialization stuff... */
	printf("Initalizing PIC               ... ");
	i8259_init();
	printf("[ OK ]\n");

	printf("Initalizing RTC               ... ");
	rtc_init(RTC_SILENT);
	printf("[ OK ]\n");

	printf("Initalizing PIT               ... ");
	pit_init();
	printf("[ OK ]\n");

	printf("Enabling Slave PIC interrupts ... ");
	enable_irq(IRQ_CAS_SIG);
	printf("[ OK ]\n");

	printf("Enabling keyboard interrupts  ... ");
	enable_irq(IRQ_KEYBOARD_CTRL);
	printf("[ OK ]\n");

	printf("Enabling mouse interrupts     ... ");
	enable_irq(IRQ_MOUSE_PS2);
	printf("[ OK ]\n");

	printf("Enabling RTC interrupts       ... ");
	enable_irq(IRQ_RTC);
	printf("[ OK ]\n");

	printf("Enabling PIT interrupts       ... ");
	enable_irq(IRQ_PIT);
	printf("[ OK ]\n");

	/* Display the splash screen */
	splash_screen();

	/* Enable interrupts */
	sti();

	/* Enable paging */
	init_paging();

	/* Enable mouse */
	mouse_init();

	/* Initialize filesystem */
	fs_init(fs_start);

	/* Clear the screen for use */
	clear_screen();

	/* Execute the first program (`shell') ... */
	execute((uint8_t*) "shell");

	/* Spin (nicely, so we don't chew up cycles) */
	asm volatile(".1: hlt; jmp .1;");
}
