import string
import re

from evdev import InputDevice
from select import select

keys = "X^1234567890XXXXqwertzuiopXXXXasdfghjklXXXXXyxcvbnmXXXXXXXXXXXXXXXXXXXXXXX"
dev = InputDevice('/dev/input/by-id/usb-GASIA_PS2toUSB_Adapter-event-kbd')


current = ""
while True:
   r,w,x = select([dev], [], [])
   for event in dev.read():
        if event.type==1 and event.value==1:
		char = keys[ event.code ]
		current += str(char)
		#print (current)
		match = re.search('X(\w+)X', current)
		if match:
			print( match.group(1))
			current = ""
