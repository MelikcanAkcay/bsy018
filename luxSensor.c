#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include </home/bsy018/project/bsy018/luxSensor.h>

void measureLux(){
    int handle = wiringPiI2CSetup(0x23);
    wiringPiI2CWrite(handle, 0x23);

    sleep(1);

    int word = wiringPiI2CReadReg16(handle, 0x00);
    int lux = ((word & 0xff00) >> 9) | ((word & 0x00ff) << 8);
    printf("Aktuelle Beleuchtungsstaerke in Lux:%d\n", lux);
    return;
}