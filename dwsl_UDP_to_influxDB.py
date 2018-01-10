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
    #print what we got
    print ("Recieved Message:", data, "From address:")
    
    # Return the parsed version of the data
    return parse_data(data)

def parse_data(data_in):
    #TODO write code that parses the given format into a NetID, Node ID, Data Type, and Data Value
    data_out = data_in.split("/")
    return data_out

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
        parsed_data = listen_UDP(UDP_IP, UDP_PORT)
        on_message(parsed_data[1], parsed_data[2], parsed_data[3], parsed_data[4])

if __name__ == '__main__':
    main()
