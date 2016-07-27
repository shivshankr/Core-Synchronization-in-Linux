import socket
TCP_IP = '192.168.100.122'
TCP_PORT = 10003
BUFFER_SIZE = 1024  # Normally 1024, but we want fast response
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)
conn, addr = s.accept()
print 'Connection address:', addr
file = open("core3.csv", "w")
while 1:
	data = conn.recv(BUFFER_SIZE)
	if not data: break
	print "received data:", data
	file.write(data)
	#conn.send(data)  # echo
file.close()
conn.close()
