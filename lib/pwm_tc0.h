#include <avr/io.h>

//Definições de macros para o trabalho com bits
#define SET_BIT(y,bit)  (y|=(1<<bit))   //coloca em 1 o bit x da variável Y

#define PWM_A PD6   //Pino que será utilizado o PWM

void set_tc0_pwm_a();           
void pwm_tc0_a(unsigned char v);