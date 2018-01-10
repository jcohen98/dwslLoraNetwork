#!/usr/bin/env python

"""
This is a simple script that is meant to recieve GNURadio UDP Messages through python and send them to influxDB

"""
from influxdb import InfluxDBClient
import socket

UDP_IP = "0.0.0.0"
UDP_PORT = 0000
HOST_DB = ''
PORT_DB = ''
USER_DB = ''
PW_DB = ''
DB_NAME = ''

def listen_UDP(IP, PORT):
    data = None
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((IP,PORT))
    while data == None:
	    data,addr = sock.recvfrom(0000)
	    print ("Recieved Message:", data)

def on_message(NetID, ID, TYPE, value):
        msg_json = [
                    {
                    "measurement": "LABTEST",
                    "tags":{
                        "NodeID": ID,
                        "SensorType": TYPE
                        },
                    "fields":{
                        "Network ID": NetID,
                        "Value": value
                        }      
                    }
                ]
def main():
    while True:
        on_message(listen_UDP(UDP_IP,UDP_PORT))

if __name__ == '__main__':
    main()
