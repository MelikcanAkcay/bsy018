#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *piCamfunc(){

    time_t rawtime;
    struct tm *info;
    char buffer[80];
    char src[40], dst[40];

    time( &rawtime );
    info = localtime( &rawtime );
    strftime(buffer,80,"%I:%M:%S%p", info);

    strcpy(dst, "cd ~/project/bsy018/piCamDir ; ./piCam ");
    strcpy(src, buffer);
    printf("\n%s.jpg\n", src);


    strcat(dst, src);
    system(dst);
    return NULL;
}