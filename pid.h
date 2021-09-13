/* 
 * File: PID.h  
 * Author: Juan Bustamante Muñoz
 * Comments:
 * Revision history: 
 */

#ifndef PID_H
#define	PID_H
#include <xc.h> // include processor files - each processor file is guarded.

void pid_init(void); // 
void adc_init(void);
int adc1_get(void);
int adc2_get(void);
int pid1(int medicion);
int pid2(int medicion);
void pwm_init(void);
void pwm1(int ciclo);
void pwm2(int ciclo);
void pid_off(void);
void relay_init(void);
void modo_manual(void);
void modo_pid(void);
void modo_remoto(void);

int kp, ti, td, r1, r2;
int error1_acumulado = 0;
int error1_previo = 0;
int error2_acumulado = 0;
int error2_previo = 0;

void pid_init(void){ // habilitar interrupciones del timer0
    error1_acumulado = 0;
    error1_previo = 0;
    error2_acumulado = 0;
    error2_previo = 0;
    INTCON = 0;
    TMR0 = 0; // interrumpe cada 0.0217, TMR0 = 162 interrumpe cada 1ms
    OPTION_REGbits.T0SE = 0;
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1; // poner en 1, estaba en 0
    OPTION_REGbits.PS0 = 1; // poner en 1, estaba en 0
    INTCONbits.RBIE = 0;
    INTCONbits.PEIE = 0;
    INTCONbits.INTE = 0;
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.GIE = 1;
}

void adc_init(void){
    TRISA |= 0x03; // AN0 y AN1 como entrada
    // Formato justificado a la izq
    ADCON1bits.ADFM = 1;
    // Clock 1:16
    ADCON1bits.ADCS2 = 1;
    ADCON0bits.ADCS1 = 0;
    ADCON0bits.ADCS0 = 1;
    // Puertos ANX entradas analogas
    ADCON1bits.PCFG = 0;
    // Prender el ADC
    ADCON0bits.ADON = 1;
}

int adc1_get(void){
    // Pin AN0 como entrada
    ADCON0bits.CHS = 0;
    int resul;
    __delay_us(20);
    ADCON0bits.GO_DONE = 1; // inicia adc
    while(1){
        if(ADCON0bits.GO_DONE==0){
            resul = ADRESH;
            resul = resul << 8;
            resul = resul + ADRESL;
            resul = resul*5;
            //altura = ((resul*26) >> 3) - 60000;
            //resul = (resul*205) >> 11;
            return resul;
            break;
        }
    }    
}

int adc2_get(void){
    // Pin AN1 como entrada
    ADCON0bits.CHS = 0b001;
    int resul;
    __delay_us(20);
    ADCON0bits.GO_DONE = 1; // inicia adc
    while(1){
        if(ADCON0bits.GO_DONE==0){
            resul = ADRESH;
            resul = resul << 8;
            resul = resul + ADRESL;
            resul = resul*5;
            //altura = ((resul*26) >> 3) - 60000;
            //resul = (resul*205) >> 11;
            return resul;
            break;
        }
    }    
}

int pid1(int medicion){
    int ciclo;
    int error;
    int proporcional;
    int integral;
    int derivativo;
    error = r1 - medicion;
    error1_acumulado = error1_acumulado + error;
    proporcional = kp*error;
    integral = ti*error1_acumulado;
    derivativo = td*(error - error1_previo);
    error1_previo = error;
    ciclo = proporcional + integral + derivativo;
    if (ciclo > 1023){ // anti wind-up
        ciclo = 1023;
        error1_acumulado = 0;
    }
    else if (ciclo < 0){
        ciclo = 0;
        error1_acumulado = 0;
    }
    return ciclo;
}

int pid2(int medicion){
    int ciclo;
    int error;
    int proporcional;
    int integral;
    int derivativo;
    error = medicion - r2;
    //error = error/10;
    error2_acumulado = error2_acumulado + error;
    proporcional = kp*error;
    integral = ti*error2_acumulado;
    derivativo = td*(error - error2_previo);
    error2_previo = error;
    ciclo = proporcional + integral + derivativo;
    if (ciclo > 1023){ // anti wind-up
        ciclo = 1023;
        error2_acumulado = 0;
    }
    else if (ciclo < 0){
        ciclo = 0;
        error2_acumulado = 0;
    }
    return ciclo;
}

void pwm_init(void){
    PR2 = 0xFF; 
    TRISC &= 0xF9; // Puerto C como salida
    // Configuracion Timer2
    T2CONbits.T2CKPS = 0b00; // Prescaler 1:16
    T2CONbits.TOUTPS = 0b0000; // Postscaler 1:1
    T2CONbits.TMR2ON = 1;
    // CCP1
    CCP1CONbits.CCP1M = 0b1100; // Seleccion modulo PWM
    CCP2CONbits.CCP2M = 0b1100;
}

void pwm1(int ciclo){
    unsigned char ocho_primeros;
    unsigned char segundo;
    unsigned char primero;
    ocho_primeros = (ciclo>>2)&0xFF;
    segundo = (ciclo>>1)&0x01;
    primero = (ciclo)&0x01;
    // Ciclo de trabajo 
    CCPR1L = ocho_primeros;
    CCP1CONbits.CCP1X = segundo;
    CCP1CONbits.CCP1Y = primero;
}

void pwm2(int ciclo){
    unsigned char ocho_primeros;
    unsigned char segundo;
    unsigned char primero;
    ocho_primeros = (ciclo>>2)&0xFF;
    segundo = (ciclo>>1)&0x01;
    primero = (ciclo)&0x01;
    // Ciclo de trabajo 
    CCPR2L = ocho_primeros;
    CCP2CONbits.CCP2X = segundo;
    CCP2CONbits.CCP2Y = primero;
}

void pid_off(void){
    INTCONbits.GIE = 0;
    INTCONbits.T0IE = 0;
    CCP1CONbits.CCP1M = 0b0000; // 
    CCP2CONbits.CCP2M = 0b0000;
    ADCON0bits.ADON = 0;
}

void relay_init(void){
    TRISC &= 0b00000110;
    PORTC |= 0b11111001;
}

void modo_manual(void){
    PORTCbits.RC3 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC0 = 0;
    PORTCbits.RC5 = 0;
}

void modo_pid(void){
    PORTCbits.RC3 = 1;
    PORTCbits.RC6 = 1;
    PORTCbits.RC4 = 1;
    PORTCbits.RC7 = 1;
}

void modo_remoto(void){
    PORTCbits.RC3 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC0 = 1;
    PORTCbits.RC5 = 1;
    PORTCbits.RC4 = 0;
    PORTCbits.RC7 = 0;
}

#endif