#ifndef DMA_H
#define DMA_H

#include <lib/lib.h>

/* DMA 1 Port Defines */
#define DMA_O_MASK	0x0A
#define DMA_O_MODE	0x0B
#define DMA_O_CLER	0x0C
#define DMA_O_BASE	0x00

/* DMA 2 Port Defines */
#define DMA_T_MASK	0xD4
#define DMA_T_MODE	0xD6
#define DMA_T_CLER	0xD8
#define DMA_T_BASE	0xC0

#define DMA_T_DMSK 	0xFE

/* DMA Memory Ports */
#define DMA_CHAN_0	0x87
#define DMA_CHAN_1	0x83
#define DMA_CHAN_2	0x81
#define DMA_CHAN_3	0x82
#define DMA_CHAN_4	0x8F
#define DMA_CHAN_5	0x8B
#define DMA_CHAN_6	0x89
#define DMA_CHAN_7	0x8A

#define DMA_CHUNK	(32*1024)


uint32_t init_dma(uint8_t channel, uint32_t addr, uint32_t size, int8_t mode);
void set_dma_rate(uint32_t d_data, uint32_t rate);
void set_dma_mem_addr(uint32_t d_data, uint32_t addr);
void clear_dma_regs(uint32_t d_data);
void set_dma_mem(uint32_t d_data, uint8_t mem);
void set_dma_mode(uint32_t d_data, uint8_t mode);
void stop_dma(uint32_t d_data);
void start_dma(uint32_t d_data);

#endif /* DMA_H */
