/* speaker.c - Implementation for speaker functions
 */

#include <drivers/speaker.h>

/**
 * Uses the internal speaker to play a sound at the specified frequency
 * @param frequency frequency to play at, will be down sampled
 */
void speaker_play_sound(uint32_t frequency) {
	// Function variables
	uint32_t divider;
	uint8_t temp;
	// Get the speaker frequency value
	divider = BASE_FREQUENCY / frequency;
	// Initalize the speaker and send the frequency over
	outb(SPKR_INIT_VALU, SPKR_INIT_PORT);
	outb((uint8_t)(divider), SPKR_FREQ_PORT);
	outb((uint8_t)(divider >> 8), SPKR_FREQ_PORT);
	//Get speaker current state
	temp = inb(SPKR_GATE_PORT);
	// If the speaker is muted, enable it
	if(temp != (temp | 3)) {
		// Enable speaker
		outb((temp | 3), SPKR_GATE_PORT);
	}
}

/**
 * Mutes the speaker
 */
void speaker_mute(void) {
	uint8_t temp = inb(SPKR_GATE_PORT) & SPKR_MUTE_VALU;
 	outb(temp, SPKR_GATE_PORT);
}

/**
 * Plays a single beep, good for errors
 */
void speaker_single_beep(void) {
	int i;
	
	for (i = 0; i < 100000; i++)
	{
		speaker_play_sound(BEEP_FREQUENCY);
	}
	speaker_mute();
}

/**
 * Plays a nice boot up melody
 * Thanks Nintendo!
 */
void sound_bootup(void) {
	int i;
	for(i = 0; i < QUARTER_NOTE; i++) {
		speaker_play_sound(E3);
	}
	for (i = 0; i < EIGHTH_NOTE; ++i)
	{
		speaker_mute();
	}
	for(i = 0; i < QUARTER_NOTE; i++) {
		speaker_play_sound(E3);
	}
	for (i = 0; i < EIGHTH_NOTE; ++i)
	{
		speaker_mute();
	}
	for(i = 0; i < EIGHTH_NOTE; i++) {
		speaker_play_sound(E3);
	}
	for (i = 0; i < EIGHTH_NOTE; ++i)
	{
		speaker_mute();
	}
	for(i = 0; i < EIGHTH_NOTE; i++) {
		speaker_play_sound(C3S);
	}
	for (i = 0; i < EIGHTH_NOTE; ++i)
	{
		speaker_mute();
	}
	for(i = 0; i < HALF_NOTE; i++) {
		speaker_play_sound(A3);
	}
	speaker_mute();
}
