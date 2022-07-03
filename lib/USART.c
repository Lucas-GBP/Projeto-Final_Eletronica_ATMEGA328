#include <USART.h>


void USART_Inic(unsigned int ubrr0){
    UBRR0H = (unsigned char)(ubrr0>>8); //Ajusta a taxa de transmissão
    UBRR0L = (unsigned char)ubrr0;
    UCSR0A = 0;                         //desabilitar velocidade dupla (no Arduino é habilitado por padrão)
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);     //Habilita a transmissão e a recepção
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);   //modo assíncrono, 8 bits de dados, 1 bit deparada, sem paridade
}

void USART_Transmite(unsigned char dado){
    while (!( UCSR0A & (1<<UDRE0)) );   //espera o dado ser enviado
    UDR0 = dado;                        //envia o dado
}

unsigned char USART_Recebe(){
    while (!(UCSR0A & (1<<RXC0)));  //espera o dado ser recebido
    return UDR0;                    //retorna o dado recebido
}

void escreve_USART(char *c){
    for (; *c!=0;c++) USART_Transmite(*c);
}

void escreve_USART_Flash(const char *c){
    for (;pgm_read_byte(&(*c))!=0;c++) USART_Transmite(pgm_read_byte(&(*c)));
}

void ident_num(unsigned int valor, unsigned char *disp){
    unsigned char n;
    for(n=0; n<tam_vetor; n++)
        disp[n] = 0 + conv_ascii; //limpa vetor para armazenagem dos digitos
    
    do{
        *disp = (valor%10) + conv_ascii; //pega o resto da divisão por 10
        valor /=10; //pega o inteiro da divisão por 10
        disp++;
    }while (valor!=0);
}

void num_USART(int num){
    if(num < 0){
        num = num*(-1);
        USART_Transmite('-');
    }
    unsigned char valures[tam_vetor];
    ident_num(num, valures);
    char i = tam_vetor;

    while(valures[i-1] == '0' && i != 1){
        i--;
    }

    for(; i > 0; i--){
        USART_Transmite(valures[i-1]);
    }
}