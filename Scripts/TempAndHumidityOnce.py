#!/usr/bin/python

import sys
import time
import Adafruit_DHT

def getTempHum():
	# intialize gpio pin
	sens = Adafruit_DHT.DHT22
	pin = 2

	# get sensor reading
	hum, temp = Adafruit_DHT.read_retry(sens, pin)

	if hum is  None:
		hum = -1
	if temp is None:
		temp = -274
	return round(hum,4), round(temp,4)
	

tempHumTuple = getTempHum();
print(getTempHum())
with open('humData.txt', 'w') as tempFile:
	tempFile.write(str(tempHumTuple[0]))
with open('tempData.txt', 'w') as humFile:
	humFile.write(str(tempHumTuple[1]))

sys.exit(1)
