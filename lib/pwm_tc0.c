#include "pwm_tc0.h"

void set_tc0_pwm_a(){
    SET_BIT(TCCR0A, WGM00);     //Modo de PWM rápido, TOP = 0xFF
    SET_BIT(TCCR0A, WGM01);     //

    SET_BIT(TCCR0A, COM0A1);    //Modo PWM rápido (modo não invertido) PD6 / IC0A

    SET_BIT(TCCR0B, CS01);      //Preescaler = 8;
    //TCCR0A = 0b10000011;
    //TCCR0B = 0b00000010;

    OCR0A = 0x00;
}

void pwm_tc0_a(unsigned char v){
    OCR0A = v;
}