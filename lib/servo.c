#include <servo.h>

void set_tc1_servo(){
    TCCR1A  = 0b10000010;
    TCCR1B  = 0b00011010;

    ICR1    = 39999;
    OCR1A   = 1000;
}

void position_servo(uint8_t position){
    OCR1A = 1000 + position*22;

    _delay_ms(25);
}