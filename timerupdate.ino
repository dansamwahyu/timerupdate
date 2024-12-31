#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"

// Define clock frequency
#define F_CPU 1000000UL

// Variables to store time
volatile uint8_t seconds = 0;
volatile uint8_t minutes = 0;
volatile uint8_t hours = 0;

// Timer0 overflow interrupt service routine
ISR(TIMER0_OVF_vect) {
    static uint16_t count = 0;
    count++;
    if (count >= 61) { // Adjust to get 1-second delay
        count = 0;
        seconds++;
        if (seconds >= 60) {
            seconds = 0;
            minutes++;
            if (minutes >= 60) {
                minutes = 0;
                hours++;
                if (hours >= 24) {
                    hours = 0;
                }
            }
        }
    }
}

void timer0_init() {
    // Set Timer0 with prescaler 1024
    TCCR0 = (1 << CS02) | (1 << CS00);
    // Enable Timer0 overflow interrupt
    TIMSK = (1 << TOIE0);
    // Initialize counter
    TCNT0 = 0;
}

// Function to display time on LCD
void display_time() {
    char time[9];
    sprintf(time, "%02d:%02d:%02d", hours, minutes, seconds);
    lcd_gotoxy(0, 0);
    lcd_puts(time);
}

int main(void) {
    // Initialize LCD
    lcd_init(LCD_DISP_ON);
    lcd_clrscr();
    
    // Initialize Timer0
    timer0_init();
    
    // Enable global interrupts
    sei();
    
    while (1) {
        display_time();
        _delay_ms(1000); // Update display every 1 second
    }
    
    return 0;
}