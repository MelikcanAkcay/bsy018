#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#include "luxSensor.h"
#include "ledMatrix.h"
#include "piCam.h"

#define RUNTIME 600                          //die zeit wie lange das Programm läuft in Sekunden
#define LUXLIMIT 3700                       //Lux Limit um es einfacher einstellen zu koennen

int main(void){
    pthread_t threads[3] = {0, 1, 2};       //erstelle 3 threads
    time_t end, start;

    int lastOp = plus;                     //letztes Zeichen was auf der LED Matrix angezeigt wurde
    void *retval;                           //1 wenn lux > luxLim || 0 wenn lux < luxLim

    ret_t* res = malloc(sizeof(ret_t));     //Erstelle ein typ res der form ret_t
    res->op_symbol = lastOp;                //Gebe ein was die letzte Operation war
    res->limit = LUXLIMIT;                  //Gebe ein was das Lux Limit ist
    for (int i = 0; i < 3; ++i) {
        res->arr[i] = 0;                    //Fuelle das Array mit Standartwerten 0
    }

    //Gebe die Initiierungsvariablen an
    printf("********************************************************************************\n");
    printf("\t\tRuntime: %dsec\n"
           "\t\tLast Op: %d (0 = minus, 1= plus)\n"
           "\t\tLux Limit: %d\n", RUNTIME, lastOp, LUXLIMIT);
    printf("********************************************************************************\n");

    end = start = (unsigned)time(NULL);       //start ist der Zeitpunkt ab dem das Programm startet

    while((end-start) < RUNTIME){       //Bricht ab wenn runtime in sekunden vergangen ist

        if(pthread_create(&threads[0], NULL, measureLux, (void*)res) != 0){     //starte die Messung der Lichtstaerke
            fprintf(stderr, "errno = %s\n", strerror(errno));
            exit(-1);
        }
        pthread_join(threads[0], (void**)&retval);                      //Warte auf das ende der Messung, setze den rückgabewert als retval

        ret_t* res = (ret_t*)retval;                                    //Caste den void* auf ret_t*
        int* op = &res->op_symbol;                                      //int* op soll op_symbol vom struct ret_t werden
        
        if(*op != lastOp ){                                                                     //Bedingung nur erfüllt wenn das Zeichen bei der Matrix sich veraendert hat

            if(pthread_create(&threads[1], NULL, MAXLedMatrix, (void*)op) != 0){                //Starte die ausgabe bei MAX7219, op bestimmt ob es +/- wird
                fprintf(stderr, "errno = %s\n", strerror(errno));
                exit(-1);
            }
            pthread_join(threads[1], NULL);                                                     //Warte bis die Matrix aufleuchtet

            if(pthread_create(&threads[2], NULL, piCamfunc, NULL) != 0){                        //Starte den thread für die piCam
                fprintf(stderr, "errno = %s\n", strerror(errno));
                exit(-1);
            }
            pthread_join(threads[0], NULL);                                                     //Warte bis das Foto gemacht wurde
            lastOp = *op;                                                                       //Setze die letzte Operation als lastOp
        }
        end = (unsigned)time(NULL);                     //Messe die Zeit und setze es als end
    }

    free(res);                                          //gebe den Speicher von res wieder frei
    return 0;
}