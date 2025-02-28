#ifndef SPEAKER_H
#define SPEAKER_H

#include <stdint.h>

// Functional parameters
#define SAMPLES 64
#define SPEAKER_MAX_FREQ 25000000 //è un periferico, quindi 1/4 del clock della cpu
#define FREQUENCY_FACTOR 4 // RAPPORTO TRA SPEAKER FREQUENCY E FREQUENZA CPU
#ifdef SIMULATTOR
    #define BPM 12000
#else
    #define BPM 1200
#endif

// Tracks names
#define STANDBY_TRACK 0
#define PLAYING_0_TRACK 1
#define PLAYING_1_TRACK 2
#define PLAYING_2_TRACK 3
#define PLAYING_HUNTER_TRACK 4 

// Enum for notes, each note associated with cc to wait until sinwave next sample
enum Note { 
    SILENCE = (unsigned int) 1,
    END = (unsigned int) 0,
    DO3 = (unsigned int)(SPEAKER_MAX_FREQ /  (SAMPLES * 130)),      // Do3
    DO3D = (unsigned int)(SPEAKER_MAX_FREQ / (SAMPLES * 138)),     // Do diesis3
    RE3 = (unsigned int)(SPEAKER_MAX_FREQ / 	(SAMPLES * 146)),      // Re3
    RE3D = (unsigned int)(SPEAKER_MAX_FREQ / (SAMPLES * 155)),     // Re diesis3
    MI3 = (unsigned int)(SPEAKER_MAX_FREQ / 	(SAMPLES * 164)),      // Mi3
    FA3 = (unsigned int)(SPEAKER_MAX_FREQ / 	(SAMPLES * 174)),      // Fa3
    FA3D = (unsigned int)(SPEAKER_MAX_FREQ / (SAMPLES * 185)),     // Fa diesis3
    SOL3 = (unsigned int)(SPEAKER_MAX_FREQ / (SAMPLES * 196)),     // Sol3
    SOL3D = (unsigned int)(SPEAKER_MAX_FREQ/ (SAMPLES * 207)),    // Sol diesis3
    LA3 = (unsigned int)(SPEAKER_MAX_FREQ / 	(SAMPLES * 220)),      // La3
    LA3D = (unsigned int)(SPEAKER_MAX_FREQ / (SAMPLES * 233)),     // La diesis3
    SI3 = (unsigned int)(SPEAKER_MAX_FREQ /  (SAMPLES * 246)),      // Si3
    DO4 = (unsigned int)(SPEAKER_MAX_FREQ /  (SAMPLES * 261)),      // Do4 (nota centrale)
    DO4D = (unsigned int)(SPEAKER_MAX_FREQ / (SAMPLES * 277)),     // Do diesis4
    RE4 = (unsigned int)(SPEAKER_MAX_FREQ /  (SAMPLES * 293)),      // Re4
    RE4D = (unsigned int)(SPEAKER_MAX_FREQ / (SAMPLES * 311)),     // Re diesis4
    MI4 = (unsigned int)(SPEAKER_MAX_FREQ /  (SAMPLES * 329)),      // Mi4
    FA4 = (unsigned int)(SPEAKER_MAX_FREQ /  (SAMPLES * 349)),      // Fa4
    FA4D = (unsigned int)(SPEAKER_MAX_FREQ / (SAMPLES * 370)),     // Fa diesis4
    SOL4 = (unsigned int)(SPEAKER_MAX_FREQ / (SAMPLES * 392)),     // Sol4
    SOL4D = (unsigned int)(SPEAKER_MAX_FREQ/ (SAMPLES * 415)),    // Sol diesis4
    LA4 = (unsigned int)(SPEAKER_MAX_FREQ /  (SAMPLES * 440)),      // La4 (440Hz)
    LA4D = (unsigned int)(SPEAKER_MAX_FREQ / (SAMPLES * 466)),     // La diesis4
    SI4 = (unsigned int)(SPEAKER_MAX_FREQ /  (SAMPLES * 493)),      // Si4
    DO5 = (unsigned int)(SPEAKER_MAX_FREQ /  (SAMPLES * 523)),       // Do5
		DO5D = (unsigned int)(SPEAKER_MAX_FREQ / (SAMPLES * 554))       // Do5
};

// Enum for notes duration, associated with cc

enum Duration { 
    DOUBLE = (unsigned int)		(((SPEAKER_MAX_FREQ / (BPM )) * 60 * 2 * FREQUENCY_FACTOR)),    // 2 battiti
    ONE = (unsigned int)			(((SPEAKER_MAX_FREQ / BPM) * 60 * FREQUENCY_FACTOR)),       // 1 battito
    HALF = (unsigned int)			(((SPEAKER_MAX_FREQ / (BPM * 2)) * 60 * FREQUENCY_FACTOR)),      // 1/2 battito
    QUARTER = (unsigned int)		(((SPEAKER_MAX_FREQ / (BPM * 4)) * 60 * FREQUENCY_FACTOR)),   // 1/4 battito
    EIGHT = (unsigned int)		(((SPEAKER_MAX_FREQ / (BPM * 8)) * 60 * FREQUENCY_FACTOR)),     // 1/8 battito
    SIXTEENTH = (unsigned int)		(((SPEAKER_MAX_FREQ / (BPM * 16)) * 60 * FREQUENCY_FACTOR)), // 1/16 battito
    THIRTYSECOND = (unsigned int)		(((SPEAKER_MAX_FREQ / (BPM * 32)) * 60 * FREQUENCY_FACTOR)), // 1/32 battito
    TRIPLET_HALF = (unsigned int)		(((SPEAKER_MAX_FREQ / (BPM * 3)) * 60 * FREQUENCY_FACTOR)) // 1/3 battito
};

// Dichiarazioni delle variabili globali
typedef struct {
    enum Note note;
    enum Duration duration;
} Melody;

extern volatile const uint16_t SinTable[SAMPLES];
extern volatile uint8_t trackSelector;
extern volatile int8_t noteIndex;
extern volatile uint8_t sinIndex;

void initSpeaker(void);
void setTrack(int trackNum);
void playTrack(int repeat);
inline uint8_t getTrackNum(){return trackSelector;};
void pauseTrack();
void resumeTrack();
void nextNote();
void nextSin();

#endif // SPEAKER_H
