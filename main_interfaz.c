/*
 * File:   main_lcd.c
 * Author: sebaa
 *
 * Created on 9 de abril de 2021, 16:38
 */

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#include "flex_lcd.h"
#include "teclado.h"
#include "pid.h"
#include <stdio.h>
#include <pic16f877a.h>
#define _XTAL_FREQ 12000000

void main(void) {
    int param = 0;
    int uno_k = 0;
    int dos_k = 0;
    int tres_k = 0;
    int altura = 0;
    int cuatro_k = 0;
    float buffer = 0;
    int referencia = 0;
    char i = 1;
    char j = 1;
    char unidad = 0;
    char tecla = 0;
    char teclita[2];
    relay_init();
    teclado_init();
    Lcd_Init();
    Lcd_Cmd(LCD_CLEAR);
    Lcd_Cmd(LCD_CURSOR_OFF);
    __delay_ms(50);
    Lcd_Out(1, 0, "Seleccione el Modo");
    Lcd_Out(2, 0, "[A]Manual");
    Lcd_Out(3, 0, "[B]Control PID");
    Lcd_Out(4, 0, "[C]Remoto");
    while(tecla == 0){
        tecla = teclado_getc();
        if (tecla == 65 || tecla == 66 || tecla == 67){
            __delay_ms(120);
        } 
        else{
            if(tecla != 0){
                tecla = 0;
                __delay_ms(120);
            }    
        }
    }
    switch(tecla){
        case 65:
            modo_manual();
            PORTB &= 0b11110000;
            Lcd_Cmd(LCD_CLEAR);
            Lcd_Out(1, 0, "Modo Manual"); 
            Lcd_Out(2, 0, "Accione las bombas"); 
            Lcd_Out(3, 0, "usando las perillas"); 
            Lcd_Out(4, 0, "[-]Menu"); 
            Lcd_Cmd(LCD_CURSOR_OFF);
            INTCONbits.RBIF = 0;
            INTCONbits.RBIE = 1;
            SLEEP();
            INTCONbits.RBIE = 0;
            break;
        case 66:
            modo_pid();
            while (j <= 3){
                i = 1;
                Lcd_Cmd(LCD_CLEAR);
                Lcd_Out(2, 0, "[A]Aceptar");
                Lcd_Out(3, 0, "[B]Menu");
                Lcd_Out(4, 0, "[C]Borrar");
                if (j == 1){
                    /*if (dec == 0){
                        Lcd_Out(4, 0, "[D]OM: Kp x 1");
                        Lcd_Out(1, 0, "Valor Kp:");
                    }
                    else {
                        Lcd_Out(4, 0, "[D]OM: Kp x 0.01");
                        Lcd_Out(1, 0, "Valor Kp:");
                    }*/
                    Lcd_Out(1, 0, "Valor Kp:");
                }
                else if (j == 2){
                    /*if (dec == 0){
                        Lcd_Out(4, 0, "[D]OM: Ki x 1");
                        Lcd_Out(1, 0, "Valor Ki:");
                    }
                    else {
                        Lcd_Out(4, 0, "[D]OM: Ki x 0.01");
                        Lcd_Out(1, 0, "Valor Ki:");
                    }*/
                    Lcd_Out(1, 0, "Valor Ki:");
                }
                else {
                    /*if (dec == 0){
                        Lcd_Out(4, 0, "[D]OM: Kd x 1");
                        Lcd_Out(1, 0, "Valor Kd:");
                    }
                    else {
                        Lcd_Out(4, 0, "[D]OM: Kd x 0.01");
                        Lcd_Out(1, 0, "Valor Kd:");
                    }*/
                    Lcd_Out(1, 0, "Valor Kd:");
                }
                Lcd_Cmd(LCD_BLINK_CURSOR_ON);
                //__delay_ms(100);
                while (i <= 4){ //5
                    tecla = 0;
                    while(tecla == 0){
                        tecla = teclado_getc();
                        if (tecla == 68 || tecla == 35 || tecla == 42){ //68
                            tecla = 0;
                            __delay_ms(120);
                        }
                        else { 
                            if (tecla != 0){
                                __delay_ms(120);
                            }    
                        } 
                    }
                    if (tecla == 66){
                        break;
                    }
                    /*else if (tecla == 68){
                        if (dec == 0){
                            dec = 1;
                            j = j - 1;
                            break;
                        }
                        else {
                            dec = 0;
                            j = j - 1;
                            break;
                        }
                    }*/
                    else if (tecla == 65){
                        if (i == 1){
                            param = 0;
                        }
                        else if (i == 2){
                            //if (dec == 0){
                            param = uno_k;
                            /*} 
                            else {
                                param = uno_k/100;
                            }*/
                        }
                        else if (i == 3){
                            //if (dec == 0){
                            param = uno_k*10 + dos_k;
                            /*}
                            else {
                                param = uno_k/10 + dos_k/100;
                            }*/
                        }
                        else if (i == 4){
                            //if (dec == 0){
                            param = uno_k*100 + dos_k*10 + tres_k;
                            /*}
                            else {
                                param = uno_k + dos_k/10 + tres_k/100;
                            }*/
                        }
                        /*else {
                            if (dec == 0){
                                param = uno_k*1000 + dos_k*100 + tres_k*10 + cuatro_k;
                            }
                            else {
                                param = uno_k*10 + dos_k + tres_k/10 + cuatro_k/100;
                            }
                        }*/
                        break;
                    }
                    else if ((tecla == 67) && (i != 1)){
                        Lcd_Out(1, 7+i, " ");
                        Lcd_Cmd(LCD_MOVE_CURSOR_LEFT);
                        i = i - 2;
                    }
                    else if ((tecla == 67) && (i == 1)){
                        i = i - 1;
                    }
                    else {
                        if (i == 4){ // 5
                            i = i - 1;
                        }
                        else {
                            sprintf(teclita, "%c", tecla);
                            Lcd_Out2(1, 8+i, teclita);
                            if (i == 1){
                                uno_k = char_to_int(tecla);
                            }
                            else if (i == 2){
                                dos_k = char_to_int(tecla);
                            }
                            else if (i == 3){
                                tres_k = char_to_int(tecla);
                            }
                            /*else {
                                cuatro_k = char_to_int(tecla);
                            }*/
                        }
                    }
                    i = i + 1;
                }
                if (tecla == 66){
                    break;
                }
                else if ((j == 1) && (tecla != 66)){
                    kp = param;
                }
                else if ((j == 2) && (tecla != 66)){
                    ti = param;
                }
                else {
                    td = param;
                }
                j = j + 1;
            }
            if (tecla == 66){
                break;
            }
            __delay_ms(50);
            j = 1;
            while (j <= 4){
                i = 1;
                if (j == 1){
                    Lcd_Cmd(LCD_CLEAR);
                    Lcd_Out(2, 0, "Altura 2");
                    Lcd_Out(3, 0, "[A]Ok [B]Menu");
                    Lcd_Out(4, 0, "[C]Borrar");
                    Lcd_Out(4, 10, "[D]Cambiar");
                    Lcd_Out(1, 0, "Altura 1");
                    if (unidad == 0){
                        Lcd_Out(3, 14, "[#]mV");
                        Lcd_Out(2, 9, "[mm]:");
                        Lcd_Out(1, 9, "[mm]:");
                    }
                    else {
                        Lcd_Out(3, 14, "[#]mm");
                        Lcd_Out(2, 9, "[mV]:");
                        Lcd_Out(1, 9, "[mV]:");
                    }
                    Lcd_Cmd(LCD_BLINK_CURSOR_ON);
                }
                else if (j == 2){
                    Lcd_Out(j, 14, " ");
                    Lcd_Cmd(LCD_MOVE_CURSOR_LEFT);
                }
                else if (j == 3){
                    Lcd_Out(4, 0, "[D]Cambiar alturas  ");
                    Lcd_Out(3, 0, "[B]Menu [C]Parar   ");
                    Lcd_Cmd(LCD_CURSOR_OFF);
                }
                else {
                    //PORTAbits.RA3 = 0;
                    Lcd_Out(3, 11, "Seguir");
                }
                while (i <= 5){
                    tecla = 0;
                    while(tecla == 0){
                        tecla = teclado_getc();
                        if (tecla == 42){
                            tecla = 0;
                            __delay_ms(120);
                        }
                        else if ((tecla != 68) && (j == 3) && (tecla != 66) && (tecla != 67)){
                            tecla = 0;
                            __delay_ms(120);
                        }
                        else if ((tecla != 68) && (j == 4) && (tecla != 66) && (tecla != 67)){
                            tecla = 0;
                            __delay_ms(120);
                        }
                        else if (tecla == 35 && j > 2){
                            tecla = 0;
                            __delay_ms(120);
                        }
                        else{
                            if (tecla != 0){
                                __delay_ms(120);
                            }
                        }
                    }
                    if (tecla == 66){
                        break;
                    }
                    else if (tecla == 35 && unidad == 0){
                        unidad = 1;
                        j = 0;
                        break;
                    }
                    else if (tecla == 35 && unidad == 1){
                        unidad = 0;
                        j = 0;
                        break;
                    }
                    else if ((tecla == 68) && (j == 1)){
                        j = j - 1;
                        break;
                    }
                    else if ((tecla == 68) && (j == 2)){
                        j = j - 2;
                        break;
                    }
                    else if ((tecla == 68) && (j == 3)){
                        j = j - 3;
                        break;
                    }
                    else if ((tecla == 68) && (j == 4)){
                        j = j - 4;
                        break;
                    }
                    else if (tecla == 65){
                        if (i == 1){
                            param = 0;
                        }
                        else if (i == 2){
                            param = uno_k;
                        }
                        else if (i == 3){
                            param = uno_k*10 + dos_k;
                        }
                        else if (i == 4){
                            param = uno_k*100 + dos_k*10 + tres_k;
                        }
                        else {
                            param = uno_k*1000 + dos_k*100 + tres_k*10 + cuatro_k;
                        }
                        break;
                    }
                    else if ((tecla == 67) && (i != 1) && (j != 3) && (j != 4)){
                        Lcd_Out(j, 12+i, " ");
                        Lcd_Cmd(LCD_MOVE_CURSOR_LEFT);
                        i = i - 2;
                    }
                    else if ((tecla == 67) && (i == 1) && (j != 3) && (j != 4)){
                        i = i - 1;
                    }
                    else if ((tecla == 67) && (j == 3)){
                        pid_off();
                        //PORTAbits.RA3 = 0;
                        break;
                    }
                    else if ((tecla == 67) && (j == 4)){
                        adc_init();
                        pwm_init();
                        pid_init();
                        j = j - 2;
                        break;
                    }
                    else {
                        if (i == 5){
                            i = i - 1;
                        }
                        else {
                            sprintf(teclita, "%c", tecla);
                            Lcd_Out2(j, 13+i, teclita);
                            if (i == 1){
                                uno_k = char_to_int(tecla);
                            }
                            else if (i == 2){
                                dos_k = char_to_int(tecla);
                            }
                            else if (i == 3){
                                tres_k = char_to_int(tecla);
                            }
                            else {
                                cuatro_k = char_to_int(tecla);
                            }
                        }
                    }
                    i = i + 1;
                }
                if (tecla == 66){
                    break;
                } 
                else if ((j == 1) && (tecla == 65)){
                    if (unidad == 0){
                        altura = param;
                        buffer = (float)param + 58.4f;
                        buffer = buffer/82.2f;
                        buffer = buffer*1000;
                        referencia = (int)buffer;
                        if (referencia > 5115 || param > 999) { //referencia
                            Lcd_Cmd(LCD_CLEAR);
                            Lcd_Out(1, 0, "La altura maxima");
                            Lcd_Out(2, 0, "permitida por el");
                            Lcd_Out(3, 0, "microcontrolador es");
                            Lcd_Out(4, 0, "5115 [mV]");
                            __delay_ms(1500);
                            j = j - 1;
                        }
                        else {
                            r1 = referencia; //caracterizar
                        }
                    }
                    else {
                        if (param > 5115) { //referencia
                            Lcd_Cmd(LCD_CLEAR);
                            Lcd_Out(1, 0, "La altura maxima");
                            Lcd_Out(2, 0, "permitida por el");
                            Lcd_Out(3, 0, "microcontrolador es");
                            Lcd_Out(4, 0, "5115 [mV]");
                            __delay_ms(1500);
                            j = j - 1;
                        }
                        else {
                            r1 = param; //caracterizar
                        }
                    }
                }
                else if ((j == 2) && (tecla == 65)){
                    if (unidad == 0){
                        buffer = (float)param + 59.5f;
                        buffer = buffer/84.5f;
                        buffer = buffer*1000;
                        referencia = (int)buffer;
                        if (referencia > 5115 || param > 999) { //999 arbitrario, necesario porque si param es muy grande referencia "da la vuelta" ya que llega solo a 32767 (int)
                            Lcd_Cmd(LCD_CLEAR);
                            Lcd_Out(1, 0, "La altura maxima");
                            Lcd_Out(2, 0, "permitida por el");
                            Lcd_Out(3, 0, "microcontrolador es");
                            Lcd_Out(4, 0, "5115 [mV]");
                            __delay_ms(1500);
                            j = j - 2;
                        }
                        else if (referencia <= 5115 && altura < param) { //referencia
                            Lcd_Cmd(LCD_CLEAR);
                            Lcd_Out(1, 0, "La altura 1 debe");
                            Lcd_Out(2, 0, "ser mayor que la");
                            Lcd_Out(3, 0, "altura 2");
                            __delay_ms(1500);
                            j = j - 2;
                        }
                        else {
                            r2 = referencia; //caracterizar
                            /*Lcd_Cmd(LCD_CLEAR);
                            sprintf(teclita, "%u", kp);
                            Lcd_Out2(1, 0, teclita);
                            sprintf(teclita, "%u", ti);
                            Lcd_Out2(1, 10, teclita);
                            sprintf(teclita, "%u", td);
                            Lcd_Out2(2, 0, teclita);
                            sprintf(teclita, "%u", r1);
                            Lcd_Out2(3, 0, teclita);
                            sprintf(teclita, "%u", r2);
                            Lcd_Out2(4, 0, teclita);
                            __delay_ms(7000);*/
                            adc_init();
                            pwm_init();
                            pid_init();
                        }
                    }
                    else {
                        if (param > 5115) { //referencia
                            Lcd_Cmd(LCD_CLEAR);
                            Lcd_Out(1, 0, "La altura maxima");
                            Lcd_Out(2, 0, "permitida por el");
                            Lcd_Out(3, 0, "microcontrolador es");
                            Lcd_Out(4, 0, "5115 [mV]");
                            __delay_ms(1500);
                            j = j - 2;
                        }
                        else if (param <= 5115 && r1 < param) { //referencia
                            Lcd_Cmd(LCD_CLEAR);
                            Lcd_Out(1, 0, "La altura 1 debe");
                            Lcd_Out(2, 0, "ser mayor que la");
                            Lcd_Out(3, 0, "altura 2");
                            __delay_ms(1500);
                            j = j - 2;
                        }
                        else {
                            r2 = param; //caracterizar
                            /*Lcd_Cmd(LCD_CLEAR);
                            sprintf(teclita, "%u", kp);
                            Lcd_Out2(1, 0, teclita);
                            sprintf(teclita, "%u", ti);
                            Lcd_Out2(1, 10, teclita);
                            sprintf(teclita, "%u", td);
                            Lcd_Out2(2, 0, teclita);
                            sprintf(teclita, "%u", r1);
                            Lcd_Out2(3, 0, teclita);
                            sprintf(teclita, "%u", r2);
                            Lcd_Out2(4, 0, teclita);
                            __delay_ms(7000);*/
                            adc_init();
                            pwm_init();
                            pid_init();
                        }
                    }
                }
                j = j + 1;
            }
            if (tecla == 66){
                pid_off();
                //PORTAbits.RA3 = 0;
            }
            break;
        case 67:
            modo_remoto();
            PORTB &= 0b11110000;
            Lcd_Cmd(LCD_CLEAR);
            Lcd_Out(1, 0, "Modo Remoto"); 
            Lcd_Out(2, 0, "Haga las conexiones"); 
            Lcd_Out(3, 0, "en la caja de bornes"); 
            Lcd_Out(4, 0, "[-]Menu"); 
            Lcd_Cmd(LCD_CURSOR_OFF);
            INTCONbits.RBIF = 0;
            INTCONbits.RBIE = 1;
            SLEEP();
            INTCONbits.RBIE = 0;
            break;
        default:
            break;
    }
    return;
}

void __interrupt() timer0(void){
    //PORTAbits.RA3 = 1;
    int med;
    int ciclo;
    med = adc1_get();
    ciclo = pid1(med);
    pwm1(ciclo);
    med = adc2_get();
    ciclo = pid2(med);
    pwm2(ciclo);
    TMR0 = 0; // TMR0 = 162, cambiar resistencia a 500 ohm (RC)
    INTCONbits.T0IF = 0;
}