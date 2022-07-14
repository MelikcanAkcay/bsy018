#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>                                                     //fuer sleep()
#include <wiringPiI2C.h>                                                //fuer die kommunikation mit dem sensor

#include "/home/bsy018/project/bsy018/luxSensor.h"                      //fuer das enum und struct ret_t

/*
 * Funktion um durchschnittliche Lux Werte der letzten 3 Messungen zu berechnen
 *
 * Input: avgArr mit 3  Int Werten
 * Output: Durchschnitt als Integer
 */


int avg(int avgArr[]){
    int avg = (avgArr[0] + avgArr[1] + avgArr[2]) / 3;
    return avg;
}

/*
 * Startfunktion fuer die Lux Messung
 *
 * Input:   void* arg welches ein struct ret_t gecastet wird
 * Output:  void* res wobei op_symbol gesetzt wird falls die gemessene Lux über dem limit ist
 *          Ebenfalls wir das Limit im struct ret_t = avg gesetzt
 */

void *measureLux(void* arg){

    ret_t* res = (ret_t*) arg;                              //Caste arg zu ret_t*
    int lux = 0, i = 0, stelle = 0;                         //Setze Initiierungswerte für die Integer

    while(i<=2){
        int handle = wiringPiI2CSetup(0x23) ;                   //Initialisiert das I2C Bus
        wiringPiI2CWrite(handle, 0x10);                         //Beginnt die Messung
        sleep(1);                                               //Warte 1 Sekunde für die Mesung
        int word = wiringPiI2CReadReg16(handle, 0x00);          //Lese den 16 Bit Wert vom Device Register
        lux = ((word & 0xff00) >> 8) | ((word & 0x00ff) << 8);  //Lese nur die letzten 8 und ersten 8 Bits vom Bus
        stelle = i%3;                                           //Stelle ist die stelle im Array wo der Lux Wert gespeichert wird, darf nur werte von 0-2 annehmen
        res->arr[stelle] = lux;                                 //Speichere den Lux Wert an der richtigen Stelle

        printf("Aktuelle Beleuchtungsstaerke: %d\n", lux);                          //Gebe den Lux Wert aus
        printf("durchschnittliche Beleuchtungsstaerke: %d\n", avg(res->arr));       //Gebe den durchschnittlichen Lux Wert aus
        i++;
        sleep(10);                                          //Warte 10 Sekunden
    }

    if(avg(res->arr) > res->limit){                         //Erfüllt nur wenn der durchschnittliche Lux Wert über dem Limit ist
        res->op_symbol = plus;                              //Setze op_symbol plus(1)
    } else{
        res->op_symbol = minus;                             //Setze op_symbol minus(0)
    }

    /*
    printf("\nNeues Limit: %d Lux\n", avg(res->arr));       //Gebe das neue Limit aus
    res->limit = avg(res->arr);                             //Setze das neue Limit
     */

    return (void*)res;                                      //Gebe res in form von einem void* wieder
}