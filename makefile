prog: main.o luxSensor.o ledMatrix.o
	gcc -o prog main.o luxSensor.o ledMatrix.o -lwiringPi

main.o: main.c
	gcc -c main.c -lpthread

luxSensor.o: luxSensor.c
	gcc -c luxSensor.c -lwiringPi

ledMatrix.o: ledMatrix.c
	gcc -c ledMatrix.c -lwiringPi