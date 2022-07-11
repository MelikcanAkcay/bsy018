#include <wiringPi.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

// define our pins :
#define DATA        27 // GPIO 17 (WiringPi pin num 0)  header pin 11
#define CLOCK       29 // GPIO 22 (WiringPi pin num 3)   header pin 15
#define LOAD        28 // GPIO 23 (WiringPi pin num 4)   header pin 16


// The Max7219 Registers :
#define DECODE_MODE   0x09
#define INTENSITY     0x0a
#define SCAN_LIMIT    0x0b
#define SHUTDOWN      0x0c
#define DISPLAY_TEST  0x0f



static void Send16bits (unsigned short output)
{
    unsigned char i;

    for (i=16; i>0; i--)
    {
        unsigned short mask = 1 << (i - 1); // calculate bitmask

        digitalWrite(CLOCK, 0);  // set clock to 0

        // Send one bit on the data pin

        if (output & mask)
            digitalWrite(DATA, 1);
        else
            digitalWrite(DATA, 0);

        digitalWrite(CLOCK, 1);  // set clock to 1

    }

}


// Take a reg numer and data and send to the max7219

static void MAX7219Send (unsigned char reg_number, unsigned char dataout)
{
    digitalWrite(LOAD, 1);  // set LOAD 1 to start
    Send16bits((reg_number << 8) + dataout);   // send 16 bits ( reg number + dataout )
    digitalWrite(LOAD, 0);  // LOAD 0 to latch
    digitalWrite(LOAD, 1);  // set LOAD 1 to finish
}

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




void *startup(void* arg)
{
    int op = *(int*) arg;
    printf("%d", op);
    printf ("\n\nRaspberry Pi Max7219 Test using WiringPi\n\n");

    if (wiringPiSetup () == -1){
        printf("error  wiringpi");
        pthread_exit(NULL);
    }
    //We need 3 output pins to control the Max7219: Data, Clock and Load
    pinMode(DATA, OUTPUT);
    pinMode(CLOCK, OUTPUT);
    pinMode(LOAD, OUTPUT);
    //MAX7219Send(SCAN_LIMIT, 7);     // set up to scan all eight digits


/*
 BCD decode mode off : data bits correspond to the segments (A-G and DP) of the seven segment display.
 BCD mode on :  0 to 15 =  0 to 9, -, E, H, L, P, and ' '
*/

    MAX7219Send(SCAN_LIMIT, 7);
    MAX7219Send(DECODE_MODE, 0);
    MAX7219Send(DISPLAY_TEST, 0);
    MAX7219Send(INTENSITY, 8);
    MAX7219Send(SHUTDOWN, 1);
    emptyDisplay();
    printf("hello");
    if(op == 0){
        minus();
    } else plus();

    pthread_exit(NULL);
}