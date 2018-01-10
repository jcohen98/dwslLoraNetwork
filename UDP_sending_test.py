#!/usr/bin/env python

import socket

# Specify UDP IP and Port to send on
UDP_IP = "127.0.0.1"
UDP_PORT = 40868

#Message
message = "TEST/0/1/TEMPC/25.1"

def send_message(MESSAGE, IP, PORT):
    # intialize and send over socket
    sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    sock.sendto(MESSAGE, (IP, PORT))

def main():
    while True:
        send_message(message, UDP_IP, UDP_PORT)

if __name__ == '__main__':
    main()
