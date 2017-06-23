import socket as soc
import time as tm

mysoc = soc.socket(soc.AF_INET, soc.SOCK_STREAM)

# connect the socket

try:
	mysoc.connect(("172.19.81.88", 502))
except soc.error, msg:
	print "Error - connect_socket"
	print 'Error Code : ' + str(msg[0]) + ' - Message ' + msg[1]
	exit()

print "socket OK"

# Modbus TCP/IP ADU:
# [2 bytes] : Transaction Identifier
# [2 bytes] : Protocol Identifier
# [2 bytes] : Lenght
# [1 byte] :  Unit Identifier
# [n bytes] : Modbus PDU

# ------- activating the leds

request = bytearray(
	[ 0x01, 0x20, 0x00, 0x00, 0x00, 0x08, 0xFA, 0x0F, 0x00, 0x11, 0x00, 0x02,
	  0x01, 0xFF])

mysoc.send("%s" % request)

response = mysoc.recv(512)

# ------- activating the motor for 2 s

request = bytearray( 
	[	0x01, 0x20, 0x00, 0x00, 0x00, 0x06, 0xFA, 0x06, 0x00, 0x0F, 0x00, 0xF0])
mysoc.send("%s" % request)
response = mysoc.recv(512)

tm.sleep(2)

request = bytearray( 
	[	0x01, 0x20, 0x00, 0x00, 0x00, 0x06, 0xFA, 0x06, 0x00, 0x0F, 0x00, 0x00])
mysoc.send("%s" % request)
response = mysoc.recv(512)
