#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// define our pins :
#define DATA        27 //DIN Pin vom Handbuch
#define CLOCK       29 //CLK Pin vom Handbuch
#define LOAD        28 //LOAD Pin vom Handbuch


// The Max7219 Registers :
#define DECODE_MODE   0x09  //Definiere die Modi vom MAX7219 nach Datenblatt
#define INTENSITY     0x0a
#define SCAN_LIMIT    0x0b
#define SHUTDOWN      0x0c
#define DISPLAY_TEST  0x0f

/*
 * Der Sendevorgang mit der LED Matrix
 *
 * Input: 16 Bits ( Die ersten 8 vom Register und 8 von den Daten)
 * Output: Nichts
 */

static void Send16bits (unsigned short output)
{
    unsigned char i;

    for (i=16; i>0; i--)
    {
        unsigned short mask = 1 << (i - 1); // calculate bitmask

        digitalWrite(CLOCK, 0);  // set clock to 0

        // Send one bit on the data pin
        if (output & mask)                          //Nur erfüllt wenn Bit in Output UND Bit in Mask 1 sind
            digitalWrite(DATA, 1);                  //Schreibe 1
        else
            digitalWrite(DATA, 0);                  //Schreibe 0

        digitalWrite(CLOCK, 1);                     // set clock to 1

    }

}


/*
 * Funktion welche den LOAD Pin richtig setzt um einen Sendevorgang einzuleiten
 *
 * Input: Das angesprochene Register und die Daten die zu versenden sind
 * Output: Nichts
 */

static void MAX7219Send (unsigned char reg_number, unsigned char dataout)
{
    digitalWrite(LOAD, 1);                      // set LOAD 1 to start
    Send16bits((reg_number << 8) + dataout);    // send 16 bits ( reg number + dataout )
    digitalWrite(LOAD, 0);                      // LOAD 0 to latch
    digitalWrite(LOAD, 1);                      // set LOAD 1 to finish
}

/*
 * Funktionen um vorgefertigte Muster auf die Matrix zu drucken
 */

void emptyDisplay(){
    MAX7219Send(1,0x00);
    MAX7219Send(2,0x00);
    MAX7219Send(3,0x00);
    MAX7219Send(4,0x00);
    MAX7219Send(5,0x00);
    MAX7219Send(6,0x00);
    MAX7219Send(7,0x00);
    MAX7219Send(8,0x00);
}

void minus(){
    MAX7219Send(1,0x18);
    MAX7219Send(2,0x18);
    MAX7219Send(3,0x18);
    MAX7219Send(4,0x18);
    MAX7219Send(5,0x18);
    MAX7219Send(6,0x18);
    MAX7219Send(7,0x18);
    MAX7219Send(8,0x18);
}

void plus(){
    MAX7219Send(1,0x18);
    MAX7219Send(2,0x18);
    MAX7219Send(3,0x18);
    MAX7219Send(4,0xff);
    MAX7219Send(5,0xff);
    MAX7219Send(6,0x18);
    MAX7219Send(7,0x18);
    MAX7219Send(8,0x18);
}


/* Anfangsfunktion für die LED Matrix
 *
 * Input: void* arg welches als ein struct ret_t gecastet wird
 * Output: Nichts
 *
 * Initialisiert sowohl wiringPi als auch die MAX7219 Led Matrix, abhaengig vom op_symbol in ret_t wird plus(1) oder minus(0) auf der Matrix gedruckt
 */

void *MAXLedMatrix(void* arg)
{
    int op = *(int*) arg;

    if (wiringPiSetup() == -1){
        printf("error  wiringpi");
        return NULL;
    }
    //We need 3 output pins to control the Max7219: Data, Clock and Load
    pinMode(DATA, OUTPUT);
    pinMode(CLOCK, OUTPUT);
    pinMode(LOAD, OUTPUT);

/*
 BCD decode mode off : data bits correspond to the segments (A-G and DP) of the seven segment display.
 BCD mode on :  0 to 15 =  0 to 9, -, E, H, L, P, and ' '
*/

    MAX7219Send(SCAN_LIMIT, 7);             //Alle Zeilen und Spalten sollen verwendet werden
    MAX7219Send(DECODE_MODE, 0);            //Nicht die vorgefertigten ausgaben verwenden sonder jede Zeile/Spalte einzeln ansteuern
    MAX7219Send(DISPLAY_TEST, 0);           //Nicht die LED's Testen
    MAX7219Send(INTENSITY, 8);              //Lichtstaerke einstellen
    MAX7219Send(SHUTDOWN, 1);               //LED's anschalten
    emptyDisplay();                         //Vorherigen Input loeschen

    if(op == 0){                            //Erfüllt falls op_symbol = 0 ist
        printf ("\nPrinting %d\n\n", 0);    //Schreibe was auf der Matrix gedruckt wird im terminal
        minus();                            //Drucke minus
    } else{
        printf ("\nPrinting %d\n\n", 1);    //Schreibe was auf der Matrix gedruckt wird im terminal
        plus();                             //Drucke minus
    }
    return NULL;
}