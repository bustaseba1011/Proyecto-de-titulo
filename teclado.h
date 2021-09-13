/*
Estas librerias fueron escritas por mi, dale like.
Instagram ➥ http://instagram.com/robertelectronica
Facebook  ➥ https://www.facebook.com/Robertelectronico/
Youtube   ➥ https://www.youtube.com/channel/UCYze8bs8C1qF6UbV2r4LrAA/videos?view_as=subscriber
*/

#ifndef KEYPAD_H
#define	KEYPAD_H
#include <xc.h>
#define PB7     7 
#define PB6     6
#define PB5     5
#define PB4     4
#define PB3     3
#define PB2     2
#define PB1     1
#define PB0     0

/* FILAS-SALIDAS */
#define ROW1   PB0
#define ROW2   PB1
#define ROW3   PB2
#define ROW4   PB3
/* COLUMNAS-ENTRADAS */
#define COL1   PB4
#define COL2   PB5
#define COL3   PB6
#define COL4   PB7

#define PTECLADO  TRISB
//char filita[4] = {PORTBbits.RB3, PORTBbits.RB2, PORTBbits.RB1, PORTBbits.RB0};
//char columnita[4] = {PORTBbits.RB7, PORTBbits.RB6, PORTBbits.RB5, PORTBbits.RB4};
#define FILAS     PORTB
#define COLUMNAS  PORTB

void teclado_init (void);
char teclado_getc(void);
int char_to_int(char x);

void teclado_init(void){
   
    PTECLADO &= ~(1<<PB0) & ~(1<<PB1) & ~(1<<PB2) & ~(1<<PB3);   // FILAS    - SALIDA
    PTECLADO |= (1<<PB4) | (1<<PB5) | (1<<PB6) | (1<<PB7);       // COLUMNAS - ENTRADA
    OPTION_REGbits.nRBPU = 0;   //   PORTB pull-ups are enabled by individual port latch values 
}

char teclado_getc(void){

char letras[4][4]={  {'1','2','3','A'},
                     {'4','5','6','B'},
                     {'7','8','9','C'},
                     {'*','0','#','D'}};
int  i=0;
char valor=0;

for(i=0;i<4;i++){

   if(i==0){

      FILAS = (1<<COL4)|(1<<COL3)|(1<<COL2)|(1<<COL1)|(1<<ROW4)|(1<<ROW3)|(1<<ROW2)|(0<<ROW1);
      __delay_us(100);
      while (!(COLUMNAS & (1<<COL1))){valor=letras[0][0];}   //1
      while (!(COLUMNAS & (1<<COL2))){valor=letras[0][1];}   //2
      while (!(COLUMNAS & (1<<COL3))){valor=letras[0][2];}   //3
      while (!(COLUMNAS & (1<<COL4))){valor=letras[0][3];}   //A
   }

   if(i==1){
      FILAS = (1<<COL4)|(1<<COL3)|(1<<COL2)|(1<<COL1)|(1<<ROW4)|(1<<ROW3)|(0<<ROW2)|(1<<ROW1);
      __delay_us(100);
      while (!(COLUMNAS & (1<<COL1))){valor=letras[1][0];}   //4
      while (!(COLUMNAS & (1<<COL2))){valor=letras[1][1];}   //5
      while (!(COLUMNAS & (1<<COL3))){valor=letras[1][2];}   //6
      while (!(COLUMNAS & (1<<COL4))){valor=letras[1][3];}   //B
   }

   if(i==2){
      FILAS = (1<<COL4)|(1<<COL3)|(1<<COL2)|(1<<COL1)|(1<<ROW4)|(0<<ROW3)|(1<<ROW2)|(1<<ROW1);
      __delay_us(100);
      while (!(COLUMNAS & (1<<COL1))){valor=letras[2][0];}   //7
      while (!(COLUMNAS & (1<<COL2))){valor=letras[2][1];}   //8
      while (!(COLUMNAS & (1<<COL3))){valor=letras[2][2];}   //9
      while (!(COLUMNAS & (1<<COL4))){valor=letras[2][3];}  //C
   }

   if(i==3){
      FILAS = (1<<COL4)|(1<<COL3)|(1<<COL2)|(1<<COL1)|(0<<ROW4)|(1<<ROW3)|(1<<ROW2)|(1<<ROW1);
      __delay_us(100);
      while (!(COLUMNAS & (1<<COL1))){valor=letras[3][0];}   //*
      while (!(COLUMNAS & (1<<COL2))){valor=letras[3][1];}   //0
      while (!(COLUMNAS & (1<<COL3))){valor=letras[3][2];}   //+
      while (!(COLUMNAS & (1<<COL4))){valor=letras[3][3];}   //D
      }
   }

   return valor;
}

int char_to_int(char x){
    int numero;
    if(x == 48){
        numero = 0;
    }
    else if(x == 49){
        numero = 1;
    }
    else if(x == 50){
        numero = 2;
    }
    else if(x == 51){
        numero = 3;
    }
    else if(x == 52){
        numero = 4;
    }
    else if(x == 53){
        numero = 5;
    }
    else if(x == 54){
        numero = 6;
    }
    else if(x == 55){
        numero = 7;
    }
    else if(x == 56){
        numero = 8;
    }
    else if(x == 57){
        numero = 9;
    }
    return numero;
}

#endif	/* KEYPAD_H */

