#ifndef _SOUNDB16_H
#define _SOUNDB16_H 

#include <lib/types.h>
#include <lib/lib.h>
#include <drivers/i8259.h>
#include <drivers/dma.h>
#include <kernel/syscall.h>

#define PLAYING			1
#define STOPPED			0
#define PAUSED			2

#define SB_PORT_OFFSET	0x220

/* Sound Blaster Playback modes */
#define SB_STATUS_PLAY	0x1
#define SB_STATUS_8BIT	0x2
#define SB_STATUS_SIGN  0x4
#define SB_STATUS_MONO  0x8
#define SB_STATUS_WRIT	0x80
#define SB_STATUS_READ	0x80
#define SB_STATUS_RATE	0x41

/* DSP Defines */
#define SB_DSP_STOP_8B	0xD0
#define SB_DSP_STOP_16	0xD5
#define SB_DSP_STPA_8B	0xDA
#define SB_DSP_STPA_16	0xD9
#define SB_DSP_PLAY_8B	0xC0
#define SB_DSP_PLAY_16	0xB0
#define SB_DSP_PAUS_8B	0xD0
#define SB_DSP_PAUS_16	0xD5
#define SB_DSP_RESM_8B	0xD4
#define SB_DSP_RESM_16	0xD6

/* DSP Mode Defines */
#define SB_DSP_PLAY_AI	0x06
#define SB_DSP_PLAY_US	0x00
#define SB_DSP_PLAY_SN	0x10
#define SB_DSP_PLAY_MN	0x00
#define SB_DSP_PLAY_ST	0x20

/* DSP Ports */
#define SB_DSP_RSET_PT	0x6
#define SB_DSP_READ_PT	0xA
#define SB_DSP_WRTE_PT	0xC
#define SB_DSP_RDST_PT	0xE
#define SB_DSP_WRST_PT	0xC
#define SB_DSP_INT_ACK	0xF

void init_sound_blaster();
void dsp_write(uint8_t data);
uint8_t dsp_read();
void sound_blaster_reset();
void sound_blaster_play(uint32_t size);
int32_t sound_blaster_pause();
int32_t sound_blaster_resume();
int32_t sound_blaster_stop();
int32_t sound_blaster_stop_op();
void sound_blaster_clear_mem();
int32_t sound_blaster_set_sample_rate(uint16_t rate);
void sound_balster_irq_handler();
void sound_blaster_play_test();

#endif /* _SOUNDB16_H */
