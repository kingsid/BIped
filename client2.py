import socket

host = '192.168.0.100'
port = 5660
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))

while True:
    command = input("Enter command: ")
    command= command.encode()
    if command == 'EXIT':
        s.send(command)
        break
    if command == 'KILL':
        s.send(command)
        break
    s.send(command)
    reply = s.recv(1024)
    print(reply)
s.close()
