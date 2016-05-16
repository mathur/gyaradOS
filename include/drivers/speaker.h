/* speaker.h - Defines for speaker functions
 */

#ifndef _SPEAKER_H
#define _SPEAKER_H

#include <lib/lib.h>
#include <lib/types.h>

/* Speaker Ports */
#define SPKR_INIT_PORT 0x43
#define SPKR_FREQ_PORT 0x42
#define SPKR_GATE_PORT 0x61

/* Speaker Values */
#define SPKR_INIT_VALU 0xB6
#define SPKR_MUTE_VALU 0xFC

/* Speaker Standard Frequencies */
#define BEEP_FREQUENCY 250
#define BASE_FREQUENCY 1193180

/* Notes for making phat beats */
/* Credit to ECE 390 Lab manual, found here:
	https://courses.engr.illinois.edu/ece390/books/labmanual/io-devices-speaker.html
*/
#define C1		130.81
#define C1S		138.59
#define D1		146.83
#define D1S		155.56
#define E1		164.81
#define F1		174.61
#define F1S		185.00
#define G1		196.00
#define G1S		207.65
#define A1		220.00
#define A1S		233.08
#define B1		246.94
#define C2		261.63
#define C2S		277.18
#define D2		293.66
#define D2S		311.13
#define E2		329.63
#define F2		349.23
#define F2S		369.99
#define G2		391.00
#define G2S		415.30
#define A2		440.00
#define A2S		466.16
#define B2		493.88
#define C3		523.25
#define C3S		554.37
#define D3		587.33
#define D3S		622.25
#define E3		659.26
#define F3		698.46
#define F3S		739.99
#define G3		783.99
#define G3S		830.61
#define A3		880.00
#define A3S		923.33
#define B3		987.77
#define C4		1046.50

#define HALF_NOTE		1800000
#define QUARTER_NOTE	900000
#define EIGHTH_NOTE		450000

/* External functions */
void speaker_play_sound(uint32_t sound_freq);
void speaker_mute(void);
void speaker_single_beep(void);
void sound_bootup(void);

#endif	/* _SPEAKER_H */
