/*
 * speaker.h 
 * 
 * API for interfacing with the MSOE dev board piezo speaker
 */

//Include Guards 
#ifndef SPEAKER_H_
#define SPEAKER_H_

// TIM3_CH1 pin assignmenr for GPIOB
#define PB4 4
#define TIM3_BASE (volatile uint32_t*) 0x40000400
#define CLK_SPEED 16000000L

typedef struct {
	float note;
    uint16_t duration;
} Tone;

/**
 * Initialize the Speaker GPIO port and peripheral clocks.
 */
void speaker_init();

/**
 * Plays a tone on the speaker
 * 
*/
void play_tone(Tone * tone);




// The following defines are from this instructable: 
// https://www.instructables.com/id/How-to-easily-play-music-with-buzzer-on-arduino-Th/
// NB: ALL NOTES DEFINED WITH STANDARD ENGLISH NAMES
#define	C0  (float) 1000000
#define	Db0	(float) 17.32
#define	D0	(float) 18.35
#define	Eb0	(float) 19.45
#define	E0	(float) 20.60
#define	F0	(float) 21.83
#define	Gb0	(float) 23.12
#define	G0	(float) 24.50
#define	Ab0	(float) 25.96
#define	A0	(float) 27.50
#define	Bb0	(float) 29.14
#define	B0	(float) 30.87
#define	C1	(float) 32.70
#define	Db1	(float) 34.65
#define	D1	(float) 36.71
#define	Eb1	(float) 38.89
#define	E1	(float) 41.20
#define	F1	(float) 43.65
#define	Gb1	(float) 46.25
#define	G1	(float) 49.00
#define	Ab1	(float) 51.91
#define	A1	(float) 55.00
#define	Bb1	(float) 58.27
#define	B1	(float) 61.74
#define	C2	(float) 65.41
#define	Db2	(float) 69.30
#define	D2	(float) 73.42
#define	Eb2	(float) 77.78
#define	E2	(float) 82.41
#define	F2	(float) 87.31
#define	Gb2	(float) 92.50
#define	G2	(float) 98.00
#define	Ab2	(float) 103.83
#define	A2	(float) 110.00
#define	Bb2	(float) 116.54
#define	B2	(float) 123.47
#define	C3	(float) 130.81
#define	Db3	(float) 138.59
#define	D3	(float) 146.83
#define	Eb3	(float) 155.56
#define	E3	(float) 164.81
#define	F3	(float) 174.61
#define	Gb3	(float) 185.00
#define	G3	(float) 196.00
#define	Ab3	(float) 207.65
#define	A3	(float) 220.00
#define	Bb3	(float) 233.08
#define	B3	(float) 246.94
#define	C4	(float) 261.63
#define	Db4	(float) 277.18
#define	D4	(float) 293.66
#define	Eb4	(float) 311.13
#define	E4	(float) 329.63
#define	F4	(float) 349.23
#define	Gb4	(float) 369.99
#define	G4	(float) 392.00
#define	Ab4	(float) 415.30
#define	A4	(float) 440.00
#define	Bb4	(float) 466.16
#define	B4	(float) 493.88
#define	C5	(float) 523.25
#define	Db5	(float) 554.37
#define	D5	(float) 587.33
#define	Eb5	(float) 622.25
#define	E5	(float) 659.26
#define	F5	(float) 698.46
#define	Gb5	(float) 739.99
#define	G5	(float) 783.99
#define	Ab5	(float) 830.61
#define	A5	(float) 880.00
#define	Bb5	(float) 932.33
#define	B5	(float) 987.77
#define	C6	(float) 1046.50
#define	Db6	(float) 1108.73
#define	D6	(float) 1174.66
#define	Eb6	(float) 1244.51
#define	E6	(float) 1318.51
#define	F6	(float) 1396.91
#define	Gb6	(float) 1479.98
#define	G6	(float) 1567.98
#define	Ab6	(float) 1661.22
#define	A6	(float) 1760.00
#define	Bb6	(float) 1864.66
#define	B6	(float) 1975.53
#define	C7	(float) 2093.00
#define	Db7	(float) 2217.46
#define	D7	(float) 2349.32
#define	Eb7	(float) 2489.02
#define	E7	(float) 2637.02
#define	F7	(float) 2793.83
#define	Gb7	(float) 2959.96
#define	G7	(float) 3135.96
#define	Ab7	(float) 3322.44
#define	A7	(float) 3520.01
#define	Bb7	(float) 3729.31
#define	B7	(float) 3951.07
#define	C8	(float) 4186.01
#define	Db8	(float) 4434.92
#define	D8	(float) 4698.64
#define	Eb8	(float) 4978.03
// DURATION OF THE NOTES 
#define BPM 120    //  you can change this value changing all the others
#define H 2*Q //half 2/4
#define Q 60000/BPM //quarter 1/4 
#define E Q/2   //eighth 1/8
#define S Q/4 // sixteenth 1/16
#define W 4*Q // whole 4/4

#endif
