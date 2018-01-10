#!/usr/bin/env python

"""
This is a simple script that is meant to recieve GNURadio UDP Messages through python and send them to influxDB

"""
from influxdb import InfluxDBClient
import socket

#Specify UDP IP and Port to listen to
UDP_IP = "127.0.0.1"
UDP_PORT = 40868

#Specifies the Database properties to send the information toi
#TODO make the database and figure out these details
HOST_DB = ''
PORT_DB = ''
USER_DB = ''
PW_DB = ''
DB_NAME = ''


def listen_UDP(IP, PORT):
    #initialize and bind the socket
    data = None
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((IP,PORT))

    #while there is no data, keep checking for data. When we find some, exit
    while data == None:
	    data,addr = sock.recvfrom(1024)

    #When I run this I want to check the type of addr so I kow what I'm dealing with (removing later)
    print(type(addr))
    print ("Recieved Message: %s", data, "From address: %s")
    
    # Return the parsed version of the data
    return parse_data(data)

def parse_data(data_in)
    #TODO write code that parses the given format into a NetID, Node ID, Data Type, and Data Value
    return data_in

def on_message(NetID, ID, TYPE, value):
    #Convert the recieved data into a readable json format by the influxDB database
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
    #TODO send the json message to the database
def main():
    while True:
        on_message(listen_UDP(UDP_IP,UDP_PORT))

if __name__ == '__main__':
    main()
