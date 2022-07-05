#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pthread.h>
#include <malloc.h>

#include </home/bsy018/project/bsy018/luxSensor.h>

#define LIMIT 1000

enum led{
    plus = 1,
    minus = 0
};

long avg(long avgArr[]){
    long avg = (avgArr[0] + avgArr[1] + avgArr[2]) / 3;
    return avg;
}

void *measureLux(){
    int lux = 0;
    int i = 0, stelle = 0;
    long avgArr[3] = {0, 0, 0};

    while(i<=3){
        int handle = wiringPiI2CSetup(0x23);
        wiringPiI2CWrite(handle, 0x10);
        int word = wiringPiI2CReadReg16(handle, 0x00);
        lux = ((word & 0xff00) >> 8) | ((word & 0x00ff) << 8);
        stelle = i%3;
        avgArr[stelle] = lux;

        printf("Aktuelle Beleuchtungsstaerke in Lux:%d\n", lux);
        printf("durchschnittliche Beleuchtungsstaerke in Lux:%d\n", avg(avgArr));
        i++;
    }
    if(avg(avgArr) > 2000){
        return (void*) plus;
    } else{ return (void*) minus;}
}