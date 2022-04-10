TARGET= rover

SOURCES= network.c morsecode.c temp_hum.c audio.c potential_meter.c audioMixer_template.c rover.c light_sampler.c rover_motor.c accelerometer.c joyStickControl.c sleep.c fileHandler.c leds.c led_display.c


PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB

all: wav
	$(CC_C) $(CFLAGS) $(SOURCES) -o $(OUTDIR)/$(TARGET) $(LFLAGS) -lpthread -lasound
	arm-linux-gnueabihf-gcc -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror capture.c -o $(OUTDIR)/capture

wav:
	mkdir -p $(PUBDIR)/rover_sounds/
	mkdir -p $(PUBDIR)/temp_hum/
	cp -R rover_sounds/* $(PUBDIR)/rover_sounds/
	cp -R temp_hum/* $(PUBDIR)/temp_hum/

clean:
	rm -f $(OUTDIR)/$(TARGET)
	rm -f $(OUTDIR)/capture