CC_C = arm-linux-gnueabihf-gcc 
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror 

all:
	$(CC_C) $(CFLAGS) rover_motor.c -o rover_motor
	cp rover_motor ~/cmpt433/public/myApps/

clean:
	rm rover_motor