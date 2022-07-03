#include "main.h"

int main(){
    SET_BIT(DDRD, PWM_A);       //Seta pino do pwm_motor como saída
    SET_BIT(DDRB, SERVO_PWM);   //Seta pino de controle do servo motor como saída

    USART_Inic(MYUBRR);         //Inicia a comunicação USART
    setup_converter();          //Configura o conversor Analogico-Digital
    set_tc0_pwm_a();            //Configura Timer 0 para o controle do pwm do motor
    set_tc1_servo();            //Seta Timer 1 para controlar o servo motor SG90

    //Configuração do timer 2
    TCCR2B = 0b00000111;
    SET_BIT(TIMSK2, TOIE2);

    //Valores Iniciais
    pwm_tc0_a(pwm_motor);                       //Valor inicial do pwm/motor como 0, valor maximo 255
    position_servo(servo_p);                    //Posiciona servo motor em 0°
    voltage_solar = read_solar_panel_precise(); //Primeira medição da tensão no painel solar
    tempeture = read_tempeture_precise();       //Primeira medição da temperatura

    sei();                                      //Habilita interrupções

    //Loop principal do programa
    while(1){
        //Verifica a temperatura do painel
        tempeture = read_tempeture_precise();
        if(tempeture > OVERHEATING_TEMP){
            state = Overheating;
        }

        //Processa o estado
        switch (state){
            case Transmiting:
                transmiting();
                break;
            case Searching_mpp:
                search_mpp();
                break;
            case Searching_vmax:
                search_max_voltage();
                break;
            case Overheating:
                overheating();
                break;
        }

        _delay_ms(100);
    }
}

ISR(TIMER2_OVF_vect){
    cont++;

    if(cont%61 == 0){   //Transmite a cada ~1 segundo
        state = Transmiting;
    }
    if(cont > 3700){    //Procura a cada ~60 segundos
        state = Searching_vmax;
        cont = 0;
    }
}

void transmiting(){
    char string[400];

    sprintf(string, "%d mV\t%d C\t%d graus\t%d PWM", voltage_solar, tempeture, servo_p, pwm_motor/255);
    escreve_USART(string);
    USART_Transmite('\n');

    state = Searching_mpp;
}

void search_max_voltage(){
    pwm_motor = 0;
    pwm_tc0_a(pwm_motor);   //Desliga motor, para não interferir na medição

    uint8_t scan_direction = 0;
    uint16_t last_tension = 0;

    for(;scan_direction < 180; scan_direction += 15){
        position_servo(scan_direction);
        _delay_ms(100);
        uint8_t tension = read_solar_panel_precise();

        if(tension > last_tension){
            last_tension = tension;
            servo_p = scan_direction;
        }
    }

    position_servo(servo_p);
    state = Searching_mpp;
}

void search_mpp(){
    voltage_solar = read_solar_panel_precise();

    if(voltage_solar > MPP_VOLTAGE+STERESE || voltage_solar < MPP_VOLTAGE-STERESE){
        if(voltage_solar < MPP_VOLTAGE-STERESE && pwm_motor > 0){
            pwm_motor -= 10;
        } else if(voltage_solar > MPP_VOLTAGE+STERESE && pwm_motor < 250){
            pwm_motor += 10;
        }
    }

    pwm_tc0_a(pwm_motor);
}

void overheating(){
    cli();

    if(servo_p > 90){
        servo_p = 0;
    } else {
        servo_p = 180;
    }

    position_servo(servo_p);

    while(tempeture > OVERHEATING_TEMP){
        tempeture = read_tempeture_precise();
        _delay_ms(1000);
        escreve_USART("--OVERHEATING--\n");
    }

    state = Searching_vmax;
    sei();
}

uint16_t read_solar_panel(){
    return read_voltage(SOLAR_VOLT_PIN)*2;
}

uint16_t read_solar_panel_precise(){
    uint32_t tension = 0;

    for(uint8_t i = 1; i <= 100; i++){
        tension += read_solar_panel();
    }

    return (tension/100);
}

uint8_t read_tempeture(){
    return read_voltage(LM35_PIN)/10;
}

uint8_t read_tempeture_precise(){
    uint16_t tempeture = 0;

    for(uint8_t i = 1; i <= 100; i++){
        tempeture += read_voltage(LM35_PIN);
    }

    return (tempeture/1000);
}