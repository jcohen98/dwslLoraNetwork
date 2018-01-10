# dwslLoraNetwork
Lora Network for the Drexel Wireless Systems Lab including USRP SDR gateway capture 
1. System will use rpp0's gr-lora GNURadio addon to capture, demodulate, and decode lora tx signals
2. It will then use UDP transfer to send the decoded messages to a python script listening on a UDP port
3. This python script will parse the recieved information and upload to a database, which can then be queried by various visualization softwares for it's data
