# Makefile for building embedded application.
# by Brian Fraser, adapted by Denys Dziubii

# Edit this file to compile extra C files into their own programs.
TARGETS=  light_sampler.c sleep.c main.c 

OUTFILE = light_sensor
PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror 
# -pg for supporting gprof profiling.
CFLAGS += -pg

all:
	$(CC_C) $(CFLAGS) $(TARGETS) -o $(OUTDIR)/$(OUTFILE) -lpthread
	
clean:
	rm -f $(OUTDIR)/$(OUTFILE)

