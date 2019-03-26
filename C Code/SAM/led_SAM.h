/*
 * led_driver.h
 *
 * Created: 1/11/19 3:47:27 AM
 *  Author: NPoindexter
 */ 



void set_LED(int row, int column, unsigned long data);
void clear_matrix(void);
void ebi_init(void);
void clk_out(void);
void display_menu_bb(void);
void display_menu_bam(void);
void display_lose(void);
void display_win(void);