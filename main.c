#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "luxSensor.h"
#include "ledMatrix.h"

void *piCamfunc( void* arg){

    time_t rawtime;
    struct tm *info;
    char buffer[80];
    char src[40], dst[40];

    time( &rawtime );
    info = localtime( &rawtime );
    strftime(buffer,80,"%I:%M%p", info);

    strcpy(dst, "cd ~/project/bsy018/piCamDir ; ./piCam ");
    strcpy(src, buffer);

    strcat(dst, src);
    system(dst);
}


int main(void){

    pthread_t threads[3] = {1, 2, 3};

    void *retval;               //1 wenn lux >2000 || 0 wenn lux <2000

    pthread_create(&threads[1], NULL, measureLux, NULL);
    pthread_join(threads[1], (void**) &retval);
    //printf("%d\n", retval);
    startup();
    //pthread_create(&threads[2], NULL, startup, NULL);
    //pthread_create(&threads[3], NULL, piCamfunc, NULL);

    return 0;
}