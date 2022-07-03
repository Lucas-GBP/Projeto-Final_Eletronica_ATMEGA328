#include <avr/io.h>
#include <util/delay.h>

//Definições de macros para o trabalho com bits
#define SET_BIT(y,bit)  (y|=(1<<bit))   //coloca em 1 o bit x da variável Y
#define CLR_BIT(y,bit)  (y&=~(1<<bit))  //coloca em 0 o bit x da variável Y
#define CPL_BIT(y,bit)  (y^=(1<<bit))   //troca o estado lógico do bit x da variável Y
#define TST_BIT(y,bit)  (y&(1<<bit))    //retorna 0 ou 1 conforme leitura do bit

#define SERVO_PWM       PB1 //Pino para PWM do motor

void set_tc1_servo();
void position_servo(uint8_t position);