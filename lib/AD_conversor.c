#include <AD_conversor.h>

void setup_converter(){
    //AREF, tensão interna VREF desligada.
    ADMUX = (1<<REFS0);
    //habilita o AD e define um prescaler de 128 (clk_AD = F_CPU/128), 125 kHz
    ADCSRA = (1<<ADEN) | (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t read_voltage(uint8_t pin){
    switch (pin){
    case PC0:
        CLR_BIT(ADMUX, MUX0);
        CLR_BIT(ADMUX, MUX1);
        CLR_BIT(ADMUX, MUX2);
        break;
    case PC1:
        SET_BIT(ADMUX, MUX0);
        CLR_BIT(ADMUX, MUX1);
        CLR_BIT(ADMUX, MUX2);
        break;
    case PC2:
        CLR_BIT(ADMUX, MUX0);
        SET_BIT(ADMUX, MUX1);
        CLR_BIT(ADMUX, MUX2);
        break;
    case PC3:
        SET_BIT(ADMUX, MUX0);
        SET_BIT(ADMUX, MUX1);
        CLR_BIT(ADMUX, MUX2);
        break;
    case PC4:
        CLR_BIT(ADMUX, MUX0);
        CLR_BIT(ADMUX, MUX1);
        SET_BIT(ADMUX, MUX2);
        break;
    case PC5:
        SET_BIT(ADMUX, MUX0);
        CLR_BIT(ADMUX, MUX1);
        SET_BIT(ADMUX, MUX2);
        break;
    }
    SET_BIT(ADCSRA, ADSC);         //inicia a conversão
    while(TST_BIT(ADCSRA,ADSC));   //espera a conversão ser finalizada

    return ADC*5/1.023;     //Retorna o valor em mV
}