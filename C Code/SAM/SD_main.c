/*The following code is a stripped down version of a previous project, there
 * may be parts of this code that are unnecessary. This example will create
 * text file and write a few letters. If you have any questions or would like
 * to learn more how to implement this project using more extensive commands
 * please contact me at OCB456@aol.com. Thanks!
 * */

//#include <MSP430F5529.h>
#include <sam.h>
#include "ff.h"
#include "diskio.h"

void initCLK(void);
void SetVcoreUp (unsigned int level);
FRESULT WriteFile(char*, char*, WORD);
void fat_init(void);

FIL file;                                               /* Opened file object */
FATFS fatfs;                                            /* File system object */
DIRS dir;                                               /* Directory object   */
FRESULT errCode;                                        /* Error code object  */
FRESULT res;                                            /* Result object      */
UINT bytesRead;                                         /* Bytes read object  */
UINT read;                                              /* Read bytes object  */

unsigned char MST_Data,SLV_Data;
BYTE buffer[32];
int result=1;

int main(void)
{
	UINT TEST[1] = {0};
	volatile UINT cnt;
	volatile UINT readTest[15];
	/* Initialize the SAM system */
	SystemInit();
	
	fat_init();                                 //mount, set directory to read from, assign file
	unsigned int bytesWritten;
	
	//READ
	for (int i = 0; i < 15; i ++){
		errCode = f_read(&file, &TEST, 1, &cnt);
		readTest[i] = *TEST;
	}
	
	//We can't write
	//errCode = f_write(&file, "abcdefg", 32, &bytesWritten);
	
	errCode = f_close(&file);
	errCode = f_mount(0,0);
	while(1);
	
	/* Replace with your application code */
	while (1)
	{
		
	}
}

void fat_init(void){
	errCode = -1;

	while (errCode != FR_OK){                               //go until f_open returns FR_OK (function successful)
		errCode = f_mount(0, &fatfs);                       //mount drive number 0
		errCode = f_opendir(&dir, "/");				    	//root directory

		errCode = f_open(&file, "/YAS", FA_READ);
		if(errCode != FR_OK)
		result=0;                                       //used as a debugging flag
		if(errCode == FR_INT_ERR)
		f_close(&file);
	}
}