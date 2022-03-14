# Makefile for building embedded application.
# by Brian Fraser

SRC_C = rover_motor.c main.c
PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -g
# -pg for supporting gprof profiling.
#CFLAGS += -pg -no-pie

all:  motor

motor:
	$(CC_C) $(CFLAGS) $(SRC_C) -o $(OUTDIR)/motor -lpthread -lm
	
clean:
	rm -f $(OUTDIR)/motor
	
