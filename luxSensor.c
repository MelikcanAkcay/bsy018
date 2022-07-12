#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pthread.h>
#include <malloc.h>

#include </home/bsy018/project/bsy018/luxSensor.h>

long avg(long avgArr[]){
    long avg = (avgArr[0] + avgArr[1] + avgArr[2]) / 3;
    return avg;
}

void *measureLux(void* arg){

    ret_t* res = (ret_t*) arg;
    int lux = 0, i = 0, stelle = 0;

    while(i<=2){
        int handle = wiringPiI2CSetup(0x23);
        wiringPiI2CWrite(handle, 0x10);
        sleep(1);
        int word = wiringPiI2CReadReg16(handle, 0x00);
        lux = ((word & 0xff00) >> 8) | ((word & 0x00ff) << 8);
        stelle = i%3;
        res->arr[stelle] = lux;

        printf("Aktuelle Beleuchtungsstaerke: %d\n", lux);
        printf("durchschnittliche Beleuchtungsstaerke: %d\n", avg(res->arr));
        i++;
        sleep(10);
    }

    if(avg(res->arr) > res->limit){
        res->op_symbol = plus;
    } else{
        res->op_symbol = minus;
    }

    printf("\nNeues Limit: %d Lux\n", avg(res->arr));
    res->limit = avg(res->arr);

    return (void*)res;
}