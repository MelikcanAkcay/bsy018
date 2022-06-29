#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "luxSensor.h"
#include "ledMatrix.h"


int main(void){

    measureLux();
    startup();

    return 0;
}