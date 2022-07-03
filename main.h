#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "lib/pwm_tc0.h"
#include "lib/USART.h"
#include "lib/AD_conversor.h"
#include "lib/servo.h"

//Definições de macros para o trabalho com bits
#define SET_BIT(y,bit)  (y|=(1<<bit))   //coloca em 1 o bit x da variável Y
#define CLR_BIT(y,bit)  (y&=~(1<<bit))  //coloca em 0 o bit x da variável Y
#define CPL_BIT(y,bit)  (y^=(1<<bit))   //troca o estado lógico do bit x da variável Y
#define TST_BIT(y,bit)  (y&(1<<bit))    //retorna 0 ou 1 conforme leitura do bit

#define SOLAR_VOLT_PIN      PC0
#define LM35_PIN            PC5

#define MPP_VOLTAGE         6000
#define STERESE             1000
#define OVERHEATING_TEMP    50      //Temperatura em Graus° 

void transmiting();                 //Transmite os valores dos perifericos via USART
void search_max_voltage();          //Posiciona servo motor para a posição de maior tensão do painel solar
void search_mpp();                  //Altera o PWM do motor em busca da maior transferencia de potência do painel
void overheating();                 //Tenta diminuir a temperatura do painel solar alterando sua posição
uint16_t read_solar_panel();
uint16_t read_solar_panel_precise();
uint8_t read_tempeture();
uint8_t read_tempeture_precise();

enum states{Transmiting, Searching_mpp, Searching_vmax, Overheating};

volatile uint8_t state = Searching_vmax;    //Estado inicial, posicionando painel solar na maior tensão
volatile uint16_t cont = 0;                 //Serve como contador para o timer 2

uint8_t servo_p = 0;                //Posição do Servo
uint8_t pwm_motor = 0;              //PWM no motor
uint16_t voltage_solar = 0;         //Tensão do painel solar
uint8_t tempeture = 0;              //Temperatura LM35