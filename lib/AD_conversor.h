#include <avr/io.h>

//Definições de macros para o trabalho com bits
#define SET_BIT(y,bit)  (y|=(1<<bit))   //coloca em 1 o bit x da variável Y
#define CLR_BIT(y,bit)  (y&=~(1<<bit))  //coloca em 0 o bit x da variável Y
#define CPL_BIT(y,bit)  (y^=(1<<bit))   //troca o estado lógico do bit x da variável Y
#define TST_BIT(y,bit)  (y&(1<<bit))    //retorna 0 ou 1 conforme leitura do bit

void setup_converter();
uint16_t read_voltage(uint8_t pin); //Retorna o valor em mV em cima do pino dado no argumento. Maximo de 5000mV