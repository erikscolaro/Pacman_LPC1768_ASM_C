#include "speaker.h"

volatile uint8_t trackSelector = 0;
volatile int8_t noteIndex = 0;
volatile uint8_t sinIndex = 0;

volatile const uint16_t SinTable[SAMPLES] = {
    511, 561, 611, 659, 707, 752, 795, 835, 873, 906, 936, 962, 984, 1000, 1013, 1020,
    1023, 1020, 1013, 1000, 984, 962, 936, 906, 873, 835, 795, 752, 707, 659, 611, 561,
    511, 461, 411, 363, 315, 270, 227, 187, 149, 116, 86, 60, 38, 22, 9, 2,
    0, 2, 9, 22, 38, 60, 86, 116, 149, 187, 227, 270, 315, 363, 411, 461
};



volatile const Melody standBy[] = {
    {DO4, HALF},   
    {DO5, HALF},   
    {SOL4, HALF},   
    {MI4, HALF},  
    {DO5, HALF},   
    {SOL4, HALF},   
    {MI4, ONE},  
    {DO4D, HALF},   
    {DO5D, HALF},   
    {SOL4D, HALF},   
    {FA4, HALF},  
    {DO5D, HALF},   
    {SOL4D, HALF},   
    {FA4, ONE},  
    {DO4, HALF},   
    {DO5, HALF},   
    {SOL4, HALF},   
    {MI4, HALF},  
    {DO5, HALF},   
    {SOL4, HALF},   
    {MI4, ONE}, 
    {FA4D, TRIPLET_HALF},
    {SOL4, TRIPLET_HALF},
    {SOL4D, TRIPLET_HALF},
    {SOL4D, TRIPLET_HALF},
    {LA4, TRIPLET_HALF},
    {LA4D, TRIPLET_HALF},
    {LA4D, TRIPLET_HALF},
    {SI4, TRIPLET_HALF},
    {DO5, TRIPLET_HALF},
    {DO5, ONE},
    {END, ONE}
};

volatile const Melody playing_0[] = {
    {DO3, QUARTER},   
		{SOL4, QUARTER},   
		{FA4D, QUARTER},   
		{RE4D, QUARTER},  
		{END, ONE}
};

volatile const Melody playing_1[] = {
    {DO3, EIGHT},   
		{SOL4, EIGHT},   
		{FA4D, EIGHT},   
		{RE4D, EIGHT},  
		{END, ONE}
};

volatile const Melody playing_2[] = {
    {DO3, SIXTEENTH},   
		{SOL4, SIXTEENTH},   
		{FA4D, SIXTEENTH},   
		{RE4D, SIXTEENTH},  
		{END, ONE}
};

volatile const Melody playing_hunter[] = {
    {SI3, THIRTYSECOND},   
		{SI4, THIRTYSECOND},   
		{FA4D, THIRTYSECOND},   
		{RE4D, THIRTYSECOND},  
		{END, ONE}
};

