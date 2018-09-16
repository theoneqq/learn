import socket

sock = socket.socket()
sock.bind(('127.0.0.1', 8080))

sock.listen(2)

while True:
	conn, addr = sock.accept()
	while True:
		conn.send('hello client'.encode('utf8'))
		data = conn.recv(1024)
		print(data.decode('utf8'))