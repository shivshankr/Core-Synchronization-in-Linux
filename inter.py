import socket
TCP_IP = '127.0.0.1'
TCP_PORT = 10010
BUFFER_SIZE = 1024  # Normally 1024, but we want fast response
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)
conn, addr = s.accept()
print 'Connection address:', addr
while 1:
	data = raw_input('Enter f for frequency or a for amplitude: ')
	conn.send(data)
	if data =="f":
		freq=raw_input('Frequency: ')
		conn.send(freq)
	elif data == "a":
			amp=raw_input('Amplitude: ')
			conn.send(amp)
	if not data: break
	#conn.send(data)  # echo
conn.close()
