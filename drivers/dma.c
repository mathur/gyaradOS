#include <drivers/dma.h>

void start_dma(uint32_t d_data) {
	if(d_data < 4) {
		outb(d_data, DMA_O_MASK);
	} else {
		outb(d_data & 3, DMA_T_MASK);
	}
}

void stop_dma(uint32_t d_data) {
	if(d_data < 4) {
		outb(d_data | 4, DMA_O_MASK);
	} else {
		outb((d_data & 3) | 4, DMA_T_MASK);
	}
}

void set_dma_mode(uint32_t d_data, uint8_t mode) {
	if(d_data < 4) {
		outb(mode | d_data, DMA_O_MASK);
	} else {
		outb(mode | (d_data & 3), DMA_T_MASK);
	}
}

void set_dma_mem(uint32_t d_data, uint8_t mem) {
	if(d_data < 4) {
		switch(d_data) {
			case 0:
				outb(mem, DMA_CHAN_0);
				break;
			case 1:
				outb(mem, DMA_CHAN_1);
				break;
			case 2:
				outb(mem, DMA_CHAN_2);
				break;
			case 3:
				outb(mem, DMA_CHAN_3);
				break;
			case 4:
				outb(mem, DMA_CHAN_4);
				break;
			case 5:
				outb(mem, DMA_CHAN_5);
				break;
			case 6:
				outb(mem, DMA_CHAN_6);
				break;
			case 7:
				outb(mem, DMA_CHAN_7);
				break;
		}
	} else {
		switch(d_data) {
			case 0:
				outb(mem | DMA_T_DMSK, DMA_CHAN_0);
				break;
			case 1:
				outb(mem | DMA_T_DMSK, DMA_CHAN_1);
				break;
			case 2:
				outb(mem | DMA_T_DMSK, DMA_CHAN_2);
				break;
			case 3:
				outb(mem | DMA_T_DMSK, DMA_CHAN_3);
				break;
			case 4:
				outb(mem | DMA_T_DMSK, DMA_CHAN_4);
				break;
			case 5:
				outb(mem | DMA_T_DMSK, DMA_CHAN_5);
				break;
			case 6:
				outb(mem | DMA_T_DMSK, DMA_CHAN_6);
				break;
			case 7:
				outb(mem | DMA_T_DMSK, DMA_CHAN_7);
				break;
		}
	}
}

void clear_dma_regs(uint32_t d_data) {
	if(d_data < 4) {
		outb(0, DMA_O_CLER);
	} else {
		outb(0, DMA_T_CLER);
	}
}

void set_dma_mem_addr(uint32_t d_data, uint32_t addr) {
	set_dma_mem(d_data, addr >> 16);
	if(d_data < 4) {
		outb(((addr) & 0xFF), (((d_data & 3) << 1 ) + DMA_O_BASE));
		outb(((addr >> 8) & 0xFF), (((d_data & 3) << 1 ) + DMA_O_BASE));
	} else {
		outb(((addr/2) & 0xFF), (((d_data & 3) << 2 ) + DMA_T_BASE));
		outb((((addr/2) >> 8) & 0xFF), (((d_data & 3) << 2 ) + DMA_T_BASE));
	}
}

void set_dma_rate(uint32_t d_data, uint32_t rate) {
	rate--;
	if(d_data < 4) {
		outb(((rate) & 0xFF), (((d_data & 3) << 1 ) + 1 + DMA_O_BASE));
		outb(((rate >> 8) & 0xFF), (((d_data & 3) << 1 ) + 1 + DMA_O_BASE));
	} else {
		outb(((rate/2) & 0xFF), (((d_data & 3) << 2 ) + 2 + DMA_T_BASE));
		outb((((rate/2) >> 8) & 0xFF), (((d_data & 3) << 2 ) + 2 + DMA_T_BASE));
	}
}

uint32_t init_dma(uint8_t channel, uint32_t addr, uint32_t size, int8_t mode) {
	if (channel > 9) {
		return -1;
	}

	stop_dma(channel);
	clear_dma_regs(channel);
	set_dma_mode(channel, mode);
	set_dma_mem_addr(channel, addr);
	set_dma_rate(channel, size);
	start_dma(channel);

	return 0;
}
