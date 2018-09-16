import socket
sock = socket.socket()
sock.connect(('127.0.0.1', 8080))

while True:
	data = sock.recv(1024)
	print(data.decode('utf8'))
	sock.send('hello server'.decode('utf8'))