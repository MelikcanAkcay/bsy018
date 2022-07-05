#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "luxSensor.h"
#include "ledMatrix.h"

void *piCamfunc(){
    system("cd ~/project/bsy018/piCamDir");
    system("./piCam");
}


int main(void){

    pthread_t threads[3] = {1, 2, 3};

    void *retval;

    //printf("Before Thread\n");
    pthread_create(&threads[1], NULL, measureLux, NULL);
    pthread_join(threads[1], (void**) &retval);
    printf("%d\n", retval);
    pthread_create(&threads[2], NULL, startup, NULL);
    //pthread_create(&threads[3], NULL, piCamfunc, NULL);

    pthread_exit(NULL);
    return 0;
}