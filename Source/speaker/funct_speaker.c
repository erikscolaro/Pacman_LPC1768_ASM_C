#include "speaker.h"
#include "LPC17xx.h"
#include "timer/timer.h"

extern volatile Melody standBy[];
extern volatile Melody playing_0[];
extern volatile Melody playing_1[];
extern volatile Melody playing_2[];
extern volatile Melody playing_hunter[];
volatile Melody (*selectedTrack)[];

static volatile int sum;
static volatile int repeat_track;

void initSpeaker(){
	// Configura il pin 26 di GPIO0 per una funzione alternativa (funzione dipendente dal microcontrollore)
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26); // Imposta il pin 26 di GPIO0 come uscita (output)
}

void setTrack(int trackNum){
	trackSelector =  trackNum;
	switch (trackNum){
		case 0:
			selectedTrack=&standBy;
			break;
		case 1:
			selectedTrack=&playing_0;
			break;
		case 2:
			selectedTrack=&playing_1;
			break;
		case 3:
			selectedTrack=&playing_2;
			break;
		case 4:
			selectedTrack=&playing_hunter;
			break;
		default:
			selectedTrack=&standBy;
	}
	noteIndex = 0;
	sinIndex = 0;

}

//remember to init timer 3!
void playTrack(int repeat){
  disable_timer(3);
  noteIndex=0;
	sum=0;
	repeat_track=repeat;
	reset_timer(3);
	nextNote();
	enable_timer(3);
}



void pauseTrack(){
	disable_timer(3);
}
	
void resumeTrack(){
	enable_timer(3);
}
void nextNote(){
	sum=0;
	if ((*selectedTrack)[noteIndex].note == END) {
		if (repeat_track>=1){
			noteIndex=0;
			
		} else {
			noteIndex=0;
			disable_timer(3);
			return;
		}
		
	}
	//  Match register 0 is for note duration, generates an interrupt and restart the counter
	set_match_register(3,0,(*selectedTrack)[noteIndex].duration, 0,1,1);  
	// Match register 1 is if sinwawe, generates an interrupt, doesnt stops or reset the counter. 
	sum=(*selectedTrack)[noteIndex].note;
	if (sum != SILENCE){
		set_match_register(3,1,sum,0,0,1);
	}
	noteIndex++;
	
	
	
}

void nextSin(){
	sum=sum+(*selectedTrack)[noteIndex].note;
	sinIndex++;
	sinIndex=(sinIndex)%SAMPLES;
	set_match_register(3,1,sum,0,0,1);
}