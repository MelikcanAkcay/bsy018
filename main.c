#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "luxSensor.h"
#include "ledMatrix.h"
#include "piCam.h"

#define runtime 60                          //die zeit wie lange das Programm läuft in Sekunden
#define luxLimit_d 500

int main(void){
    pthread_t threads[3] = {0, 1, 2};       //erstelle 3 threads
    time_t end, start;

    int lastOp = minus;                     //letztes Zeichen was auf der LED Matrix angezeigt wurde
    void *luxLim = (void*) luxLimit_d;      //Lux limit ab dem das Zeichen umspringt
    void *retval;                           //1 wenn lux > luxLim || 0 wenn lux < luxLim


    //Gebe die Initiierungsvariablen an
    printf("********************************************************************************\n");
    printf("\t\tRuntime: %dsec\n"
           "\t\tLast Op: %d (0 = minus, 1= plus)\n"
           "\t\tLux Limit: %d\n", runtime, lastOp, (int*) luxLim);
    printf("********************************************************************************\n");

    end = start = (unsigned)time(NULL);       //start ist der Zeitpunkt ab dem das Programm startet

    while((end-start) < runtime){       //Bricht ab wenn runtime in sekunden vergangen ist

        pthread_create(&threads[0], NULL, measureLux, &luxLim); //starte die Messung der Lichtstaerke
        pthread_join(threads[0], (void**) &retval);             //Warte auf das ende der Messung, setze den rückgabewert als retval

        pthread_create(&threads[1], NULL, startup, &retval);    //Starte die ausgabe bei MAX7219, retval bestimmt ob es +/- wird
        pthread_join(threads[1], NULL);                         //Warte bis die Matrix aufleuchtet

        if((int)retval != lastOp ){                             //Bedingung nur erfüllt wenn das Zeichen bei der Matrix sich veraendert hat
            pthread_create(&threads[2], NULL, piCamfunc, NULL); //Starte den thread für die piCam
            pthread_join(threads[0], NULL);                     //Warte bis das Foto gemacht wurde
            lastOp = (int) retval;                              //Setze die letzte Operation als lastOp
        }
        end = (unsigned)time(NULL);                             //Messe die Zeit und setze es als end
    }


    return 0;
}