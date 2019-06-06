/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */

#include "lcd.h"            //Include 16x2 LCD library

unsigned int delayus = 2020;

void Crotate(int steps)
{
    //Clockwise Rotation
    for(int i=0; i < steps; i++)
    {
        PORTC = 0b10010000;
        __delay_us(delayus);
        PORTC = 0b01010000;
        __delay_us(delayus);
        PORTC = 0b01100000;
        __delay_us(delayus);
        PORTC = 0b10100000;
        __delay_us(delayus);
    }
    PORTC = 0b00000000;
}

void CWrotate(int steps)
{
    //Counter Clockwise Rotation
    for(int i=0; i < steps; i++)
    {
    PORTC = 0b10100000;
    __delay_us(delayus);
    PORTC = 0b01100000;
    __delay_us(delayus);
    PORTC = 0b01010000;
    __delay_us(delayus);
    PORTC = 0b10010000;
    __delay_us(delayus);
    }
    PORTC = 0b00000000;    
}

void main(void)
{			
    PORTA = 0;				// Clear Port
	PORTB = 0;	
	PORTC = 0;
	PORTD = 0;
	
	TRISA = 0b11111111;		// set PORTA as INPUT
	TRISB = 0b00000011;		// set PORTB<7:2> as OUTPUT . PORTB<1:0> as INPUT
	TRISC = 0b00000000;		// set PORTC as OUTPUT
	TRISD = 0b00000000;		// set PORTD as OUTPUT
    
    RB6 = 1;        //Turn on on board LED
    
    lcd_initialize();
    
    while(1)
    {
        lcd_home();
        lcd_putstr("Setting for note");
        lcd_2ndline();
        lcd_putstr("D");
        __delay_ms(1000);
        
        Crotate(200);
        __delay_ms(200);
        CWrotate(10);
        __delay_ms(200);
        CWrotate(10);
        __delay_ms(200);
        CWrotate(100);
        __delay_ms(200);
        Crotate(30);
        __delay_ms(200);
        Crotate(20);
        
        lcd_home();
        lcd_putstr("Setting for note");
        lcd_2ndline();
        lcd_putstr("A");
        __delay_ms(1000);
        
        Crotate(20);
        __delay_ms(200);
        CWrotate(100);
        __delay_ms(200);
        CWrotate(150);
        __delay_ms(200);
        CWrotate(40);
        __delay_ms(200);
        Crotate(70);
        __delay_ms(200);
        Crotate(45);
        
        lcd_home();
        lcd_putstr("Setting for note");
        lcd_2ndline();
        lcd_putstr("E");
        __delay_ms(1000);
        
        Crotate(120);
        __delay_ms(200);
        CWrotate(70);
        __delay_ms(200);
        CWrotate(150);
        __delay_ms(200);
        CWrotate(10);
        __delay_ms(200);
        Crotate(10);
        __delay_ms(200);
        Crotate(35);
        
        lcd_home();
        lcd_putstr("Setting for note");
        lcd_2ndline();
        lcd_putstr("G");
        __delay_ms(1000);
        
        Crotate(200);
        __delay_ms(200);
        CWrotate(100);
        __delay_ms(200);
        CWrotate(75);
        __delay_ms(200);
        CWrotate(30);
        __delay_ms(200);
        Crotate(150);
        __delay_ms(200);
        Crotate(250);
        
        lcd_home();
        lcd_putstr("Setting for note");
        lcd_2ndline();
        lcd_putstr("B");
        __delay_ms(1000);
        
        Crotate(20);
        __delay_ms(20);
        CWrotate(100);
        __delay_ms(15);
        CWrotate(150);
        __delay_ms(30);
        CWrotate(40);
        __delay_ms(200);
        Crotate(70);
        __delay_ms(200);
        Crotate(100);
        
        lcd_home();
        lcd_putstr("Setting for note");
        lcd_2ndline();
        lcd_putstr("E");
        __delay_ms(1000);
        
        Crotate(12);
        __delay_ms(200);
        CWrotate(10);
        __delay_ms(200);
        CWrotate(20);
        __delay_ms(200);
        CWrotate(100);
        __delay_ms(200);
        Crotate(10);
        __delay_ms(200);
        Crotate(30);
        
        __delay_ms(2000);
        lcd_clear();
        lcd_home();
        lcd_putstr("Done setting notes!");
        __delay_ms(5000);
    }
}