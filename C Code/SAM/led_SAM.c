#include "led_SAM.h"


void set_LED(int row, int column, unsigned long data){
	volatile unsigned long address = array_addressing[row][column];
	
	volatile unsigned long *address_start =  0x60000000;
	

	*(address + address_start) = data;

	
}

void clear_matrix(void){
	for (int i = 0; i<32; i++){
		for(int j = 0; j<32; j++){
			set_LED(i,j,0);
		}
	}
} 

void ebi_init(void){
	
	//Disable Pins from PDIO
	
	//REG_PIOC_WPMR = PIO_WPMR_WPKEY(PIO_WPMR_WPKEY_PASSWD);
	
	//REG_PIOC_PDR = 0xFFFFFFFF;
	
	REG_PIOC_PDR = PIO_PER_P18 |  PIO_PER_P19 | PIO_PER_P20 | PIO_PER_P21 | PIO_PER_P22 | PIO_PER_P23 | PIO_PER_P24 | PIO_PER_P25 |
					 PIO_PER_P26 | PIO_PER_P27 | PIO_PER_P28 | PIO_PER_P29 | PIO_PER_P30 | PIO_PER_P31 | PIO_PER_P0 | PIO_PER_P1 |
					  PIO_PER_P2 | PIO_PER_P3 | PIO_PER_P4 | PIO_PER_P5 | PIO_PER_P6 | PIO_PER_P7 | PIO_PER_P8 | PIO_PER_P9 |
						 PIO_PER_P30 | PIO_PER_P31 | PIO_PER_P10 | PIO_PER_P11 | PIO_PER_P13 | PIO_PER_P14;
		
	//REG_PIOC_PER = 0xFFFFFFFF;
	
	
	//Enable Peripheral	
		 
	REG_PIOC_ABCDSR &=  ~(PIO_PER_P18 |  PIO_PER_P19 | PIO_PER_P20 | PIO_PER_P21 | PIO_PER_P22 | PIO_PER_P23 | PIO_PER_P24 | PIO_PER_P25 |
							PIO_PER_P26 | PIO_PER_P27 | PIO_PER_P28 | PIO_PER_P29 | PIO_PER_P30 | PIO_PER_P31 | PIO_PER_P0 | PIO_PER_P1 |
								PIO_PER_P2 | PIO_PER_P3 | PIO_PER_P4 | PIO_PER_P5 | PIO_PER_P6 | PIO_PER_P7 | PIO_PER_P8 |  PIO_PER_P11 | 
									PIO_PER_P30 | PIO_PER_P31 | PIO_PER_P14 | PIO_PER_P9 | PIO_PER_P13 | PIO_PER_P10 );
								
								
	//CCFG REgister?
	REG_CCFG_SMCNFCS = CCFG_SMCNFCS_SMC_NFCS0;
	
	//Disabling Write Protection Register
	REG_SMC_WPMR = SMC_WPMR_WPKEY_PASSWD;
	
	//SMC Setup Length
	REG_SMC_SETUP0  = SMC_SETUP_NCS_RD_SETUP(1) + SMC_SETUP_NRD_SETUP(1) + SMC_SETUP_NCS_WR_SETUP(1) + SMC_SETUP_NWE_SETUP(1);
	
	//SMC Pulse Length
	REG_SMC_PULSE0 = SMC_PULSE_NCS_RD_PULSE(2) + SMC_PULSE_NRD_PULSE(2) + SMC_PULSE_NCS_WR_PULSE(2) + SMC_PULSE_NWE_PULSE(1);
	
	//SMC Cycle Length 
	REG_SMC_CYCLE0 = SMC_CYCLE_NRD_CYCLE(4) + SMC_CYCLE_NWE_CYCLE(4);
	
	REG_SMC_MODE0 = SMC_MODE_PS(SMC_MODE_PS_8_BYTE) + SMC_MODE_PMEN;
	
	//SMC Mode
	//REG_SMC_MODE0
	/*
	//Enabling Write Protection
	REG_SMC_WPMR = SMC_WPMR_WPKEY_PASSWD + SMC_WPMR_WPEN;
	
	*/
	
	clk_out();
	
}

void clk_out(void){
	
	//REG_CKGR_MOR = CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCF(CKGR_MOR_MOSCRCF_12_MHz);
	
	REG_PIOA_PDR = PIO_PER_P6;
	
	REG_PIOA_ABCDSR = PIO_PER_P6;
	
	REG_PMC_PCK |= PMC_PCK_CSS(PMC_PCK_CSS_MCK);
	
	REG_PMC_SCER |= PMC_SCER_PCK0;
	
}