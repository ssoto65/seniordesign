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

//#define BB 1
//extern volatile uint8_t game;

#define paddleLEFT 1
#define paddleRIGHT 2

extern volatile uint8_t paddleDir;
volatile uint8_t paddleIdx = 13;

volatile int LeftOrRight = 0; //left = 0, right =1
volatile int UpOrDown = 0; //up= 0, down=1
uint8_t xPos = 30;
uint8_t yPos = 15;

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
	
	//game = BB;
	
	bb_screen_load();
	bb_init_ball();
	bb_init_paddle();
	
	//start our TC0
	REG_TC0_CCR0 |= TC_CCR_SWTRG;
}

void paddleRefresh(void){
	if(paddleDir == paddleRIGHT){
		paddleDir = 0;
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
		paddleDir = 0;
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
	set_LED(xPos,yPos,0x00); //Remove current ball position
	
	//Check Column boundaries
	if (yPos == 28){
		LeftOrRight = Left;
	}
	else if (yPos == 3){
		LeftOrRight = Right;
	}
	
	//Check Row boundaries
	if (xPos == 30){
		if ((yPos+1 >= paddleIdx) && (yPos+1 <= paddleIdx+5)){
			UpOrDown = Up;
		}
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
	
	volatile uint8_t current = bb_level_one[xPos][yPos];
	//Brick hit
	if (current == 2){
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
	}
	
	//Update ball location
	set_LED(xPos,yPos,0x00FFFFFF);
}

void bb_screen_load(void){
	
	clear_matrix();
	for(int i=0; i < 50; i++){
		for(int ii = 0; ii < 32; ii++){
			//ii = 0;
			for(int jj = 0; jj < 32; jj++){
				if(bb_level_one[ii][jj]== 1){
					set_LED(ii,jj,0xFF0000);
				}
				else if(bb_level_one[ii][jj]== 2){
					set_LED(ii,jj,0x0000FF);
				}
				else if(bb_level_one[ii][jj]== 3){
					set_LED(ii,jj,0x00FF00);
				}
			}
		}
	}
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
