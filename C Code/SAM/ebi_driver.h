/*//////////////////////////////////////////////////////////////////////////////////
 * ebi_driver.h
 *
 *  Modified: 24 March 2014
 *  Author:  Dr. Schwartz, Ivan Bukreyev and Josh Weaver
 *  Purpose: Header file to hold functions for far memory address
 *           read and writes.
 *///////////////////////////////////////////////////////////////////////////////////

#define WE_bm       PIN0_bm
#define RE_bm       PIN1_bm
#define ALE1_bm     PIN2_bm
#define CS1_bm      PIN5_bm

#define LED_ARRAY_START 0x004000

#ifndef EBI_DRIVER_H
#define EBI_DRIVER_H

/************************************************************************************
* Name:     __far_mem_read
* Purpose:  Read value from far memory address of three bytes.
*           See below for explanation of "asm volitile( );"
* Inputs:	addr (32-bit)
* Output:	result (8-bit)
************************************************************************************/
#define __far_mem_read(addr)                    \
            (__extension__({                    \
            uint32_t temp32 = (uint32_t)(addr); \
            uint8_t result;                     \
            asm volatile(                       \
                "in __tmp_reg__, %2"     "\n\t" \
                "out %2, %C1"            "\n\t" \
                "movw r30, %1"           "\n\t" \
                "ld %0, Z"               "\n\t" \
                "out %2, __tmp_reg__"    "\n\t" \
                : "=r" (result)                 \
                : "r" (temp32),                 \
                  "I" (_SFR_IO_ADDR(RAMPZ))     \
                : "r30", "r31"                  \
            );                                  \
            result;                             \
        }))
/*******************************************************************************
Explanation for the above asm volatile ()
   "in __tmp_reg__, %2"     "\n\t" \	// Load temp register with RAMPZ
   "out %2, %C1"            "\n\t" \	// Store bits 23:16 of 'addr' as RAMPZ
   "movw r30, %1"           "\n\t" \	// Copy Lower 16 bits of 'addr' into Z register
   "ld %0, Z"               "\n\t" \	// Read 'data' from location pointed by Z register
   "out %2, __tmp_reg__"           \	// Restore previous RAMPZ
   : "=r" (result)                 \	// Output-operand-list (first column). The result %0
   : "r" (temp32),                 \	// List-input-operand (second column). Operand for placeholder %1
     "I" (_SFR_IO_ADDR(RAMPZ)),    \	// Operand for placeholder %2
   : "r30", "r31"                  \	// Clobber-list (third column). 
********************************************************************************/

/************************************************************************************
* Name:     __far_mem_write
* Purpose:  Read value from far memory address of three bytes.  
*           See below for explanation of "asm volitile( );"
* Inputs:	addr (32-bit), data (8-bit)
* Output:	write to memory
************************************************************************************/
#define __far_mem_write(addr, data)             \
            (__extension__({                    \
            uint32_t temp32 = (uint32_t)(addr); \
            asm volatile(                       \
                "in __tmp_reg__, %1"     "\n\t" \
                "out %1, %C0"            "\n\t" \
                "movw r30, %0"           "\n\t" \
                "st Z, %2"               "\n\t" \
                "out %1, __tmp_reg__"           \
                :                               \
                : "r" (temp32),                 \
                  "I" (_SFR_IO_ADDR(RAMPZ)),    \
                  "r" ((uint8_t)data)           \
                : "r30", "r31"                  \
            );                                  \
        }))
/*******************************************************************************
Explanation for the above asm volatile ()
   "in __tmp_reg__, %1"     "\n\t" \	// Load temp register with RAMPZ
   "out %1, %C0"            "\n\t" \	// Store bits 23:16 of 'addr' as RAMPZ
   "movw r30, %0"           "\n\t" \	// Copy Lower 16 bits of 'addr' into Z register
   "st Z, %2"               "\n\t" \	// Write 'data' to location pointed by Z register
   "out %1, __tmp_reg__"           \	// Restore previous RAMPZ
   :                               \	// Output-operand-list (first column). In this case there are none.
   : "r" (temp32),                 \	// List-input-operand (second column). Operand for placeholder %0
   "I" (_SFR_IO_ADDR(RAMPZ)),      \	// Operand for placeholder %1
   "r" ((uint8_t)data)             \	// Operand for placeholder %2
   : "r30", "r31"                  \	// Clobber-list (third column). 
********************************************************************************/
#define _far_mem_enter_ISR()  uint8_t volatile saved_rampz = RAMPZ; \
                                      RAMPZ = 0;
#define __far_mem_exit_ISR()   RAMPZ = saved_rampz;

#endif /* EBI_DRIVER_H_ */

void ebi_init(void)
{
	/* Set control signals false upon startup */
	PORTH.OUTSET = WE_bm | RE_bm | CS1_bm;
	
	/* Set control signals as outputs */
	PORTH.DIRSET = WE_bm | RE_bm | ALE1_bm | CS1_bm;
	
	/* Set address lines as outputs */
	PORTK.DIRSET = UINT8_MAX;
	
	/* Select SRAM 3-PORT ALE1 mode */
	EBI.CTRL = EBI_IFMODE_3PORT_gc;
	
	//	EBI.CS0.CTRLB = EBI_CS_SRWS_7CLK_gc;
	
	/* Select EBI chip select (CS1) size */
	EBI.CS1.CTRLA = EBI_CS_ASIZE_4KB_gc | EBI_CS_MODE_SRAM_gc;
	
	/* Set base address for CS1 */
	EBI.CS1.BASEADDR = (uint16_t) LED_ARRAY_START >> 8;
}
