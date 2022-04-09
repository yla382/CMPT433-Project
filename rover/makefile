TARGET= rover

SOURCES= network.c morsecode.c rover.c light_sampler.c rover_motor.c accelerometer.c joyStickControl.c sleep.c fileHandler.c leds.c


PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror


all: wav
	$(CC_C) $(CFLAGS) $(SOURCES) -o $(OUTDIR)/$(TARGET) -lpthread 
	arm-linux-gnueabihf-gcc -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror capture.c -o $(OUTDIR)/capture

wav:
	mkdir -p $(PUBDIR)/rover_sounds/
	cp -R rover_sounds/* $(PUBDIR)/rover_sounds/ 

clean:
	rm -f $(OUTDIR)/$(TARGET)
	rm -f $(OUTDIR)/capture