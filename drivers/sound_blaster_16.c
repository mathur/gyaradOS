#include <drivers/sound_blaster_16.h>

int8_t channel;
int8_t mode;
int8_t status;
int32_t file;
static int8_t d_buf[DMA_CHUNK * 2] __attribute__((aligned(32768)));
static int8_t* fd_b_one = d_buf;
static int8_t* fd_b_two = d_buf + DMA_CHUNK;
static int8_t* fd_b_current = d_buf;

/**
 *
 * ALL INFORMATION TAKEN FROM:
 *
 * sb16doc.zip
 * sblast09.zip
 * sbfmwhn.zip
 *
 * http://ftp.lanet.lv/ftp/mirror/x2ftp/msdos/programming/mxinfo/
 *
 * and
 *
 * http://www.gamasutra.com/view/feature/3194/programming_digitized_sound_on_the_.php?print=1
 */


void init_sound_blaster() {
	channel = 1;
	mode = 0;
	status = STOPPED;
	sound_blaster_reset();
}

void dsp_write(uint8_t data) {
	while(inb(SB_DSP_WRST_PT + SB_PORT_OFFSET) & SB_STATUS_WRIT) {
		// Wait...
	}
	outb(data, SB_DSP_WRTE_PT + SB_PORT_OFFSET);
}

uint8_t dsp_read() {
	while(!(inb(SB_DSP_WRST_PT + SB_PORT_OFFSET) & SB_STATUS_READ)) {
		// Wait...
	}
	return inb(SB_DSP_READ_PT + SB_PORT_OFFSET);
}

/**
 * Steps to Reset DSP
 *
 * 1. Write a 1 to the reset port
 * 2. Wait for 3 MS
 * 3. Write a 0 to the reset prot
 * 4. Poll read-buffer status port until bit 7 is set
 * 5. Pol the read data port until you recieve an AA
 */
void sound_blaster_reset() {
	outb(1, SB_PORT_OFFSET + SB_DSP_RSET_PT);
	int i;
	for(i = 0; i < 50000; i++) {
		/* spin */
	}
	outb(0, SB_PORT_OFFSET + SB_DSP_RSET_PT);
	printf("%s\n", "WAITING FOR SB CLEAR");
	while(!(inb(SB_PORT_OFFSET + SB_DSP_RDST_PT) & 0x80)){
		
	}
	printf("%s\n", "WAITING FOR SB READ");
	while(!(inb(SB_PORT_OFFSET + SB_DSP_READ_PT) == 0xAA)){
		
	}
	return;
}

void sound_blaster_play(uint32_t size) {
	uint16_t s_block = (uint16_t) size;

	uint8_t cmd;
	if(mode & SB_STATUS_8BIT) {
		cmd |= SB_DSP_PLAY_8B;
	} else {
		cmd |= SB_DSP_PLAY_16;
	}

	cmd |= SB_DSP_PLAY_AI;
	dsp_write(cmd);
	uint8_t md = 0;
	if(mode & SB_STATUS_MONO) {
		md |= SB_DSP_PLAY_MN;
	} else {
		md |= SB_DSP_PLAY_SN;
	}
	dsp_write(md);
	s_block--;
	dsp_write(((s_block) & 0xFF));
	dsp_write(((s_block >> 8 )& 0xFF));
}

int32_t sound_blaster_pause() {
	if(status != PLAYING) {
		return -1;
	}
	if(mode & SB_STATUS_8BIT) {
		dsp_write(SB_DSP_PAUS_8B);
	} else {
		dsp_write(SB_DSP_PAUS_16);
	}

	return 0;
}

int32_t sound_blaster_resume() {
	if(status != PAUSED) {
		return -1;
	}
	if(mode & SB_STATUS_8BIT) {
		dsp_write(SB_DSP_RESM_8B);
	} else {
		dsp_write(SB_DSP_RESM_16);
	}
	
	return 0;
}

int32_t sound_blaster_stop() {
	if(status != PLAYING || status != PAUSED) {
		return -1;
	}
	if(mode & SB_STATUS_8BIT) {
		dsp_write(SB_DSP_STOP_8B);
	} else {
		dsp_write(SB_DSP_STOP_16);
	}
	
	return 0;
}

int32_t sound_blaster_stop_op() {
	if(mode & SB_STATUS_8BIT) {
		dsp_write(SB_DSP_STPA_8B);
	} else {
		dsp_write(SB_DSP_STPA_16);
	}
	
	return 0;
}

void sound_blaster_clear_mem() {
	int i;
	status = STOPPED;
	fd_b_current = fd_b_one;
	for(i = 0; i < DMA_CHUNK * 2; i++) {
		d_buf[i] = 0;
	}
	close(file);
}


int32_t sound_blaster_set_sample_rate(uint16_t rate) {
	dsp_write(SB_STATUS_RATE);
	dsp_write(((rate >> 8) & 0xFF));
	dsp_write(((rate) & 0xFF));
	return 0;
}

void sound_blaster_play_test() {
	status = PLAYING;
	mode |= SB_STATUS_8BIT;
	channel = 1;
	sound_blaster_set_sample_rate(22050);
	int i;
	for(i = 0; i < DMA_CHUNK * 2; i++) {
		d_buf[i] = ( rand() % 10824) + 1;
	}
	init_dma(channel, (uint32_t)d_buf, DMA_CHUNK * 2, SB_DSP_PLAY_AI);
	sound_blaster_play((uint16_t) DMA_CHUNK);
}

void sound_balster_irq_handler() {
	uint32_t flags;
	cli_and_save(flags);
	if(status) {
		uint32_t b_read = read(file, (uint8_t*)fd_b_current, DMA_CHUNK);
		if(b_read) {
			uint16_t fd_b_size = (uint16_t) b_read;
			if(b_read < DMA_CHUNK) {
				sound_blaster_stop_op();
				sound_blaster_clear_mem();
			}
			if(fd_b_current == fd_b_one) {
				fd_b_current = fd_b_two;
			} else {
				fd_b_current = fd_b_one;
			}
			sound_blaster_play(fd_b_size);
		} else {
			sound_blaster_clear_mem();
			sound_blaster_stop();
		}
	}
	inb(SB_DSP_RDST_PT + SB_PORT_OFFSET);
	inb(SB_DSP_INT_ACK + SB_PORT_OFFSET);
	send_eoi(IRQ_PARLL_PRT_SND);
	restore_flags(flags);
}
