#include <avr/io.h>
#include <avr/pgmspace.h>

#define BAUD 9600               //taxa de 9600 bps
#define MYUBRR F_CPU/16/BAUD-1
#define tam_vetor 5             //número de digitos individuais para a conversão por ident_num()
#define conv_ascii 48           /*48 se ident_num() deve retornar um número no formato ASCII(0 para formato normal)*/

void USART_Inic(unsigned int ubbr0);        //Inicia a comunicação USART
void USART_Transmite(unsigned char dado);   //Transmite 1byte de dados
unsigned char USART_Recebe();               //Rotarna 1bye de dados recebidos
void escreve_USART(char *c);                //escreve String (RAM)
void escreve_USART_Flash(const char *c);    //escreve String (Flash)
void ident_num(unsigned int valor, unsigned char *disp);//Conversão de um número em seus digitos individuais
void num_USART(int num);                    //Transmite o numero