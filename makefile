prog: main.o luxSensor.o ledMatrix.o piCam.o
	gcc -o prog main.o luxSensor.o ledMatrix.o piCam.o -lwiringPi -lpthread

main.o: main.c
	gcc -c main.c -lpthread

luxSensor.o: luxSensor.c
	gcc -c luxSensor.c -lwiringPi

ledMatrix.o: ledMatrix.c
	gcc -c ledMatrix.c -lwiringPi

piCam.o: piCam.c
	gcc -c piCam.c