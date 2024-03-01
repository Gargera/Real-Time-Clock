#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "LCD.h"
#include "BUTTON.h"
#include "KEYPAD_4x3.h"
#include "seven segment.h"
#include "TIMER.h"

#define F_CPU         8000000UL

volatile unsigned char seconds = 0 , hours = 0 , minutes = 0 , button_on = 0;

int main(void)
{
	LCD_init('B' , 1 , 'B' , 6 , 'B' , 7 , 'B' , 5 , 4);
	BUTTON_vInit('B' , 0);
	seven_seg_vinit('A');
	for(unsigned char i = 0 ; i <= 5 ; i++){
		DIO_vsetPINDir('C' , i , 1);
	}
    keypad_vInit('D');
	timer2_overflow_init_interrupt();
	LCD_vSend_string('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , "  Press SET to");
	LCD_movecursor(2 , 4 , 'B' , 1 , 'B' , 7 , 'B' , 5 , 4);
	LCD_vSend_string('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , "set clock");
		
    while(1)
    {
		unsigned char arr[6] = {hours/10 , hours%10 , minutes/10 , minutes%10 , seconds/10 , seconds%10};
		for(unsigned char mpx = 0 ; mpx <= 5 ; mpx++){
			for(unsigned char i = 0 ; i <= 5 ; i++){
			    if(i == mpx){
				   DIO_write('C' , i , 0);
			    }
			    else{
				   DIO_write('C' , i , 1);
			    }
			}
			
			seven_seg_write('A' , arr[mpx] , 0);
			_delay_ms(5);			
		}
		
		
		if(seconds >= 60){
			minutes++;
			seconds = 0;
		}	
		if(minutes >= 60){
			hours++;
			minutes = 0;
		}
		if(hours >= 24){
			hours = 0;
		}
		
		
		if(BUTTON_u8read('B' , 0) == 0){
			//stop seconds counting
			button_on = 1;
			
			//stop seven-segments
			for(unsigned char i = 0 ; i <= 5 ; i++){
				DIO_write('C' , i , 1);
			}
			
			//set seconds
			do{
				LCD_clearscreen('B' , 1 , 'B' , 7 , 'B' , 5 , 4);
				LCD_vSend_string('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , "Seconds = __");
				
				unsigned char x = 0 , num = 0;
				while(1){
					x = keypad_u8check_press('D');
					if(x != NOTPRESSED){
						while(keypad_u8check_press('D') == x);
						num = x - '0';
						break;
					}
				}
				LCD_movecursor(1 , 11 , 'B' , 1 , 'B' , 7 , 'B' , 5 , 4);
				LCD_vSend_char('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , x);
				
				num *= 10;
				while(1){
					x = keypad_u8check_press('D');
					if(x != NOTPRESSED){
						while(keypad_u8check_press('D') == x);
						num += x - '0';
						break;
					}
				}
				LCD_movecursor(1 , 12 , 'B' , 1 , 'B' , 7 , 'B' , 5 , 4);
				LCD_vSend_char('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , x);
				_delay_ms(3000);
				
				if(num > 59){
					LCD_clearscreen('B' , 1 , 'B' , 7 , 'B' , 5 , 4);
					LCD_vSend_string('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , " num should be");
					LCD_movecursor(2 , 1 , 'B' , 1 , 'B' , 7 , 'B' , 5 , 4);
					LCD_vSend_string('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , "between 0 and 59");
					_delay_ms(15000);
				}
				else{
					seconds = num;
					break;
				}
				
			}while(1);
			
			
			//set minutes
			do{
				LCD_clearscreen('B' , 1 , 'B' , 7 , 'B' , 5 , 4);
				LCD_vSend_string('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , "Minutes = __");
				
				unsigned char x = 0 , num = 0;
				while(1){
					x = keypad_u8check_press('D');
					if(x != NOTPRESSED){
						while(keypad_u8check_press('D') == x);
						num = x - '0';
						break;
					}
				}
				LCD_movecursor(1 , 11 , 'B' , 1 , 'B' , 7 , 'B' , 5 , 4);
				LCD_vSend_char('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , x);
				
				num *= 10;
				while(1){
					x = keypad_u8check_press('D');
					if(x != NOTPRESSED){
						while(keypad_u8check_press('D') == x);
						num += x - '0';
						break;
					}
				}
				LCD_movecursor(1 , 12 , 'B' , 1 , 'B' , 7 , 'B' , 5 , 4);
				LCD_vSend_char('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , x);
				_delay_ms(3000);
				
				if(num > 59){
					LCD_clearscreen('B' , 1 , 'B' , 7 , 'B' , 5 , 4);
					LCD_vSend_string('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , " num should be");
					LCD_movecursor(2 , 1 , 'B' , 1 , 'B' , 7 , 'B' , 5 , 4);
					LCD_vSend_string('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , "between 0 and 59");
					_delay_ms(15000);
				}
				else{
					minutes = num;
					break;
				}
				
			}while(1);
			
			
			//set hours
			do{
				LCD_clearscreen('B' , 1 , 'B' , 7 , 'B' , 5 , 4);
				LCD_vSend_string('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , "Hours = __");
				
				unsigned char x = 0 , num = 0;
				while(1){
					x = keypad_u8check_press('D');
					if(x != NOTPRESSED){
						while(keypad_u8check_press('D') == x);
						num = x - '0';
						break;
					}
				}				
				LCD_movecursor(1 , 9 , 'B' , 1 , 'B' , 7 , 'B' , 5 , 4);
				LCD_vSend_char('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , x);
				
				num *= 10;
				while(1){
					x = keypad_u8check_press('D');
					if(x != NOTPRESSED){
						while(keypad_u8check_press('D') == x);
						num += x - '0';
						break;
					}
				}
				LCD_movecursor(1 , 10 , 'B' , 1 , 'B' , 7 , 'B' , 5 , 4);
				LCD_vSend_char('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , x);
				_delay_ms(3000);
				
				if(num > 23){
					LCD_clearscreen('B' , 1 , 'B' , 7 , 'B' , 5 , 4);
					LCD_vSend_string('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , " num should be");
					LCD_movecursor(2 , 1 , 'B' , 1 , 'B' , 7 , 'B' , 5 , 4);
					LCD_vSend_string('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , "between 0 and 23");
					_delay_ms(15000);
				}
				else{
					hours = num;
					break;
				}
				
			}while(1);	
			
			
			LCD_clearscreen('B' , 1 , 'B' , 7 , 'B' , 5 , 4);
			LCD_vSend_string('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , "  Press SET to");
			LCD_movecursor(2 , 4 , 'B' , 1 , 'B' , 7 , 'B' , 5 , 4);
			LCD_vSend_string('B' , 1 , 'B' , 7 , 'B' , 5 , 4 , "set clock");	
			
			//clear button
			while(BUTTON_u8read('B' , 0) == 0);	
			
			//continue seconds counting
			button_on = 0;
		}											
    }
}

ISR(TIMER2_OVF_vect)
{
	if(button_on == 0){
	   seconds++;
	}	   
}