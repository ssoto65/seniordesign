/*
 * bam_SAM.c
 *
 * Created: 3/16/2019 11:41:38 AM
 *  Author: SamanthaSoto
 */ 

#include "sam.h"
#include "bam_SAM.h"
#include "led_SAM.h"
#include "i2c_SAM.h"

//#define BAM 0
//extern volatile uint8_t game;

uint8_t xPos;
uint8_t yPos;
extern volatile int16_t accel_x;
extern volatile int16_t accel_y;
extern volatile int16_t accel_z;

#define lose 0
#define win 1
volatile uint8_t bam_WinOrLose;

int bam_level_one[32][32] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,1,0,0,0,0,0,0,0,0,1,0,2,0,0,2,0,0,0,0,2,0,0,3,0,0,0,2,0,1},
{1,0,0,1,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2,0,0,0,2,0,3,3,3,0,2,2,2,1},
{1,0,0,1,0,0,2,2,2,2,0,0,1,0,2,0,0,2,0,0,0,0,2,0,0,3,0,0,0,2,0,1},
{1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1},
{1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1},
{1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,1},
{1,0,0,1,0,2,0,0,0,1,0,0,0,0,2,0,0,2,0,0,0,0,2,0,0,0,0,0,0,0,0,1},
{1,0,0,1,2,2,2,0,0,1,1,1,1,2,2,2,2,2,2,1,0,0,2,0,0,0,0,0,0,2,0,1},
{1,0,0,1,0,2,0,0,0,0,0,0,0,0,2,0,0,2,0,1,0,0,2,0,0,1,1,1,2,2,2,1},
{1,0,0,1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,2,0,1},
{1,0,0,1,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1},
{1,0,0,1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1},
{1,0,0,1,1,1,1,1,1,1,2,2,2,2,2,2,0,0,0,1,0,0,2,1,1,1,1,1,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,2,0,0,0,1,0,0,2,0,0,0,2,0,0,2,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,1,0,0,2,0,0,2,2,2,2,2,2,1},
{1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,1,0,0,2,0,0,0,2,0,0,2,0,1},
{1,0,2,2,2,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1},
{1,0,0,2,0,0,0,0,1,0,2,0,0,0,0,1,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,1,2,2,2,0,0,0,1,0,0,0,2,0,0,2,2,2,2,2,2,2,0,0,1},
{1,0,0,0,0,2,0,0,1,0,2,0,0,0,0,1,0,0,0,2,0,0,2,0,0,0,0,0,2,0,0,1},
{1,0,0,0,2,2,2,0,1,0,0,0,0,0,0,1,0,0,0,2,0,0,2,0,0,0,0,0,2,0,0,1},
{1,0,0,0,0,2,0,0,1,0,0,0,0,0,0,1,0,0,0,2,0,0,2,0,0,2,0,0,2,0,0,1},
{1,0,0,0,1,1,1,1,1,0,0,0,1,0,0,1,0,0,0,2,0,0,2,0,0,2,0,0,2,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,2,0,0,0,0,0,2,0,0,2,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,2,0,0,2,0,0,1},
{1,0,2,0,0,0,0,0,0,0,2,0,1,0,0,0,0,0,0,1,0,2,0,0,0,2,0,0,2,0,0,1},
{1,2,2,2,1,1,1,1,1,2,2,2,0,0,0,0,0,0,0,1,2,2,2,0,0,2,0,0,0,0,0,1},
{1,0,2,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1,0,2,0,0,0,2,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

void bam_play(void){
	//game = BAM;
	bam_screen_load();
	bam_init_ball();
	
	bam_WinOrLose = 37;
	
	//start our TC0
	REG_TC0_CCR0 &= ~TC_CCR_CLKDIS;
	REG_TC0_CCR0 |= TC_CCR_CLKEN;
	REG_TC0_CCR0 |= TC_CCR_SWTRG;
}

void bam_screen_load(void){
	
	clear_matrix();
	for(int i=0; i < 50; i++){
		for(int ii = 0; ii < 32; ii++){
			//ii = 0;
			for(int jj = 0; jj < 32; jj++){
				if(bam_level_one[ii][jj]== 1){
					set_LED(ii,jj,0xFF0000);
				}
				else if(bam_level_one[ii][jj]== 2){
					set_LED(ii,jj,0x0000FF);
				}
				else if(bam_level_one[ii][jj]== 3){
					set_LED(ii,jj,0x00FF00);
				}
			}
		}
	}
}

void bam_init_ball(void){
	//Initialize ball
	xPos = 1;
	yPos = 1;
	set_LED(xPos,yPos,0x00FFFFFF);
}

void mazerefresh(void){
	
	accel_data();
	if( (accel_x > -750) && (accel_x < 750) && (accel_y < -1600)){
		if((bam_level_one[xPos-1][yPos] == 0) ){
			set_LED(xPos,yPos,0x00);
			set_LED(--xPos,yPos,0x00FFFFFF);
		}else if((bam_level_one[xPos-1][yPos] == 2)){
			bam_WinOrLose = lose;
		}
		else if ((bam_level_one[xPos-1][yPos] == 3)){
			bam_WinOrLose = win;
		}
	}
	//LEFT
	else if( (accel_x > -850) && (accel_x < 850) && (accel_y > 500)){
		if((bam_level_one[1+xPos][yPos] == 0) ){
			set_LED(xPos,yPos,0x00);
			set_LED(++xPos,yPos,0x00FFFFFF);
		}else if((bam_level_one[1+xPos][yPos] == 2)){
			bam_WinOrLose = lose;
		}		
		else if ((bam_level_one[1+xPos][yPos] == 3)){
			bam_WinOrLose = win;
		}
	}
	//DOWN
	else if( (accel_y > -850) && (accel_y < 850) && (accel_x < -700)){
		if((bam_level_one[xPos][1+yPos] == 0) ){
			set_LED(xPos,yPos,0x00);
			set_LED(xPos,++yPos,0x00FFFFFF);
		}else if((bam_level_one[xPos][1+yPos] == 2)){
			bam_WinOrLose = lose;
		}
		//Green
		else if((bam_level_one[xPos][1+yPos] == 3)){
			bam_WinOrLose = win;
		}
	}
	//UP
	else if( (accel_y > -850) && (accel_y < 850) && (accel_x > 650)){
		//Empty Space
		if((bam_level_one[xPos][yPos-1] == 0) ){
			set_LED(xPos,yPos,0x00);
			set_LED(xPos,--yPos,0x00FFFFFF);
		//Red
		}else if((bam_level_one[xPos][yPos-1] == 2) ){
			bam_WinOrLose = lose;
		}
		//Green
		else if((bam_level_one[xPos][yPos-1] == 3) ){
			bam_WinOrLose = win;			
		}
		}
}