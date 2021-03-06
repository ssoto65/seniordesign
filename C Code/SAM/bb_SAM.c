/*
 * bb.c
 *
 * Created: 3/16/2019 11:18:13 AM
 *  Author: SamanthaSoto
 */ 

#include "sam.h"
#include "bb_SAM.h"
#include "led_SAM.h"


#define Left 0
#define Right 1
#define Up 0
#define Down 1


extern volatile uint32_t wireless_mode;

//#define BB 1
//extern volatile uint8_t game;

#define lose 0
#define win 1
volatile uint8_t bb_WinOrLose;
volatile uint8_t bb_level = 0;

uint8_t musicFlags[10];
#define bam_ball_move 0
#define game_win 1
#define game_lose 2
#define bam_wall_hit 3
#define bb_paddle_hit 4
#define bb_brick_hit 5

#define paddleLEFT 1
#define paddleRIGHT 2

extern volatile uint8_t paddleDir;
volatile uint8_t paddleIdx = 13;

volatile int LeftOrRight = 0; //left = 0, right =1
volatile int UpOrDown = 0; //up= 0, down=1
uint8_t xPos = 30;
uint8_t yPos = 15;

volatile uint8_t current;




int bb_level_one_OG[32][32] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,2,2,2,0,2,2,2,0,2,2,2,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,2,2,2,0,2,2,2,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1}}; 
	
	
int bb_level_UF_OG[32][32] ={{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,1,1,1},
{1,1,1,0,0,0,0,0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,0,0,0,0,0,1,1,1},
{1,1,1,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,1,1,1},
{1,1,1,0,0,0,0,0,4,4,4,0,4,4,4,0,4,4,4,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,0,2,2,2,0,0,1,1,1},
{1,1,1,0,0,0,0,0,4,4,4,0,4,4,4,0,4,4,4,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,1,1,1},
{1,1,1,0,0,0,0,0,4,4,4,0,4,4,4,0,4,4,4,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,4,4,4,0,0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1}};
	

	
int bb_level_one[32][32] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,2,2,2,0,2,2,2,0,2,2,2,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,2,2,2,0,2,2,2,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1}};

void bb_play(void){
	REG_TC0_CCR0 &= ~TC_CCR_CLKDIS;
	//game = BB;
	bb_WinOrLose = 37;
	//rtcSetTime(0,0,0);
	
	bb_screen_load();
	bb_init_ball();
	bb_init_paddle();
	
	//start our TC0
	REG_TC0_CCR0 |= TC_CCR_CLKEN;
	REG_TC0_CCR0 |= TC_CCR_SWTRG;
}

void paddleRefresh(void){
	if (wireless_mode == 0){
		if (((REG_PIOA_PDSR & PIO_PDSR_P18)>>18) == 0) {
			paddleDir = paddleLEFT;
		}else if (((REG_PIOA_PDSR & PIO_PDSR_P22)>>22) == 0) {
			paddleDir = paddleRIGHT;
		}else{
			paddleDir = 0;
		}
			
	}
	
	
	if(paddleDir == paddleRIGHT){
		if (paddleIdx+4 != 28){
			set_LED(31,paddleIdx,0x00);
			set_LED(31,paddleIdx+1,0x00);
			set_LED(31,paddleIdx+2,0x00);
			set_LED(31,paddleIdx+3,0x00);
			set_LED(31,paddleIdx+4,0x00);
		
			paddleIdx++;
			set_LED(31,paddleIdx,0x0000FF00);
			set_LED(31,paddleIdx+1,0x0000FF00);
			set_LED(31,paddleIdx+2,0x0000FF00);
			set_LED(31,paddleIdx+3,0x0000FF00);
			set_LED(31,paddleIdx+4,0x0000FF00);
		}
	}
	else if(paddleDir == paddleLEFT){
		if (paddleIdx != 3){
			set_LED(31,paddleIdx,0x00);
			set_LED(31,paddleIdx+1,0x00);
			set_LED(31,paddleIdx+2,0x00);
			set_LED(31,paddleIdx+3,0x00);
			set_LED(31,paddleIdx+4,0x00);
			
			paddleIdx--;
			set_LED(31,paddleIdx,0x0000FF00);
			set_LED(31,paddleIdx+1,0x0000FF00);
			set_LED(31,paddleIdx+2,0x0000FF00);
			set_LED(31,paddleIdx+3,0x0000FF00);
			set_LED(31,paddleIdx+4,0x0000FF00);
		}
			
	}
}

void ballRefresh(void){
	
	
	if (current != 4) set_LED(xPos,yPos,0x00); //Remove current ball position
	
	//Check Column boundaries
	if (yPos == 28){
		LeftOrRight = Left;
	}
	else if (yPos == 3){
		LeftOrRight = Right;
	}
	
	//Check Row boundaries
	if (xPos == 30){
		//Ball bounce to left //Hit left side of paddle
		if ((yPos+1 >= paddleIdx) && (yPos+1 <= paddleIdx+2)){
			musicFlags[bb_paddle_hit] = 1;
			//trying to fix glitch where ball goes through wall
			if(paddleIdx <= 3){
				UpOrDown = Up;
				LeftOrRight = Right;
			}else{
				UpOrDown = Up;
				LeftOrRight = Left;
			}
		//Ball bounce in same direction its going //Hit center of paddle
		}else if (yPos+1 == paddleIdx+3){
			musicFlags[bb_paddle_hit] = 1;
			UpOrDown = Up;
		}
		//Ball bounce to the right //Hit right side of paddle
		else if ((yPos+1 >= paddleIdx+4) && (yPos+1 <= paddleIdx+5)){
			musicFlags[bb_paddle_hit] = 1;
			if(paddleIdx >= 28){
				UpOrDown = Up;
				LeftOrRight = Left;
				}else{
				UpOrDown = Up;
				LeftOrRight = Right;
			}
		}
		//Ball did not hit paddle
		else{
			//endGame();
			
			//restart paddle
			set_LED(31,paddleIdx,0x00);
			set_LED(31,paddleIdx+1,0x00);
			set_LED(31,paddleIdx+2,0x00);
			set_LED(31,paddleIdx+3,0x00);
			set_LED(31,paddleIdx+4,0x00);
			bb_init_paddle();
			
			//restart Ball
			xPos = 30;
			yPos = 15;
			
			UpOrDown = Up;
			
			bb_level = 0;
			bb_WinOrLose = lose;
		}
	}
	else if (xPos == 1){
		UpOrDown = Down;
	}
	
	//Move ball on Row
	if (LeftOrRight == Left){
		yPos--;
	}
	else if (LeftOrRight == Right){
		yPos++;
	}
	
	//Move ball on Column
	if (UpOrDown == Up){
		xPos--;
	}
	else if (UpOrDown == Down){
		xPos++;
	}
	
	current = bb_level_one[xPos][yPos];
	//RED Brick hit
	if (current == 2){
		musicFlags[bam_ball_move] = 1;
		UpOrDown ^= 1; //Switch vertical direction
		if(bb_level_one[xPos][yPos+1] == 0){
			//turn off led
			set_LED(xPos,yPos,0x00);
			set_LED(xPos,yPos-1,0x00);
			set_LED(xPos,yPos-2,0x00);
			
			//update bit map
			bb_level_one[xPos][yPos] = 0;
			bb_level_one[xPos][yPos-1] = 0;
			bb_level_one[xPos][yPos-2] = 0;
		}
		else if(bb_level_one[xPos][yPos-1] == 0){
			//turn off led
			set_LED(xPos,yPos,0x00);
			set_LED(xPos,yPos+1,0x00);
			set_LED(xPos,yPos+2,0x00);
			
			//update bit map
			bb_level_one[xPos][yPos] = 0;
			bb_level_one[xPos][yPos+1] = 0;
			bb_level_one[xPos][yPos+2] = 0;
		}
		else{
			//turn off led
			set_LED(xPos,yPos-1,0x00);
			set_LED(xPos,yPos,0x00);
			set_LED(xPos,yPos+1,0x00);
			
			//update bit map
			bb_level_one[xPos][yPos-1] = 0;
			bb_level_one[xPos][yPos] = 0;
			bb_level_one[xPos][yPos+1] = 0;
		}
	musicFlags[0] = 1;
	}
	
	//YELLOW Brick hit
	if (current == 4){
		musicFlags[bam_ball_move] = 1;
		UpOrDown ^= 1; //Switch vertical direction
		if(bb_level_one[xPos][yPos+1] == 0){
			//turn led red
			set_LED(xPos,yPos,0x0000FF);
			set_LED(xPos,yPos-1,0x0000FF);
			set_LED(xPos,yPos-2,0x0000FF);
			
			//update bit map
			bb_level_one[xPos][yPos] = 2;
			bb_level_one[xPos][yPos-1] = 2;
			bb_level_one[xPos][yPos-2] = 2;
		}
		else if(bb_level_one[xPos][yPos-1] == 0){
			//turn led red
			set_LED(xPos,yPos,0x0000FF);
			set_LED(xPos,yPos+1,0x0000FF);
			set_LED(xPos,yPos+2,0x0000FF);
			
			//update bit map
			bb_level_one[xPos][yPos] = 2;
			bb_level_one[xPos][yPos+1] = 2;
			bb_level_one[xPos][yPos+2] = 2;
		}
		else{
			//turn led red
			set_LED(xPos,yPos-1,0x0000FF);
			set_LED(xPos,yPos,0x0000FF);
			set_LED(xPos,yPos+1,0x0000FF);
			
			//update bit map
			bb_level_one[xPos][yPos-1] = 2;
			bb_level_one[xPos][yPos] = 2;
			bb_level_one[xPos][yPos+1] = 2;
		}
	musicFlags[0] = 1;
	}
	
	volatile int red_found = 0;
	
	for(int ii = 0; ii <32; ii++){
		for(int jj = 0; jj <32; jj++){
			if(bb_level_one[ii][jj] == 2 || bb_level_one[ii][jj] == 4){
				red_found = 1;
			}
		}
	}
	
	if(red_found == 0 ){
		bb_level++;
		if (bb_level == 2){
			bb_WinOrLose = win;
		}
		else{
			bb_play();
		}
		
	}
	
	//Update ball location
	if (current != 4) set_LED(xPos,yPos,0x00FFFFFF);
}

void bb_screen_load(void){
	
	clear_matrix();
	//for(int i=0; i < 50; i++){
		for(int ii = 0; ii < 32; ii++){
			//ii = 0;
			for(int jj = 0; jj < 32; jj++){
				if(bb_level == 0){
					bb_level_one[ii][jj] = bb_level_one_OG[ii][jj];
				}else if (bb_level == 1){
					bb_level_one[ii][jj] = bb_level_UF_OG[ii][jj];
				}
				if(bb_level_one[ii][jj]== 1){
					set_LED(ii,jj,0xFF0000);
				}
				else if(bb_level_one[ii][jj]== 2){
					set_LED(ii,jj,0x0000FF);
				}
				else if(bb_level_one[ii][jj]== 3){
					set_LED(ii,jj,0x00FF00);
				}
				else if(bb_level_one[ii][jj]== 4){
					set_LED(ii,jj,0x026AFD);
				}
				else{
					set_LED(ii,jj,0);
				}
			}
		}
	//}
}

void bb_init_ball(void){
	//Initialize ball
	xPos = 30;
	yPos = 15;
	set_LED(xPos,yPos,0x00FFFFFF);
}

void bb_init_paddle(void){
	//Initialize paddle
	paddleIdx = 13;
	set_LED(31,paddleIdx,0x0000FF00);
	set_LED(31,paddleIdx+1,0x0000FF00);
	set_LED(31,paddleIdx+2,0x0000FF00);
	set_LED(31,paddleIdx+3,0x0000FF00);
	set_LED(31,paddleIdx+4,0x0000FF00);
}
