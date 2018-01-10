 #include <stdlib.h>
/*
MarvinLaptop
Basic controls of IoT Academy Marvin LoRa Development board through your Laptop
This version supports:
  - Sending LoRa uplink messages using ABP that are given as input from the serial port on your laptop
  - Blink three times when sending data
  - Power control to RN2903A module
Instructions:
  - Get the latest version of the Arduino software
  - In Arduino IDE select Arduino Leonardo and com port of your device
  - Please adjust ABP adresses and key below to match yours
*/
// Port to assign the type of lora data (any port can be used between 1 and 223)
//assign this to the port in the gateway
int     set_port  = 1;

// Some standard ports that depend on the layout of the Marvin
long    defaultBaudRate = 57600;
int     reset_port = 5;
int     RN2903A_power_port = 6;
int     led_port = 13;

//*** Set parameters here BEGIN ---->
String  set_nwkskey = "00000000000000000000000000000012"; // Put your 32 hex char here
String  set_appskey = "00000000000000000000000000000001"; // Put your 32 hex char here
String  set_devaddr = "00000001"; // Put your 8 hex char here
//deveui will be shown when the program is run
//*** <---- END Set parameters here

// Some global items
String reader = "";
String receive = "";
String codeRate = "4/8";
String spreadFactor = "sf8";

/*
 * Setup() function is called when board is started. Marvin uses a serial connection to talk to your pc and a serial
 * connection to talk to the RN2903A, these are both initialized in seperate functions. Also some Arduino port are 
 * initialized and a LED is called to blink when everything is done. 
 */
void setup() {
  InitializeSerials(defaultBaudRate);
  initializeRN2903A(RN2903A_power_port, reset_port);
  pinMode(led_port, OUTPUT); // Initialize LED port  
  print_to_console("Would you like to:\n1.Enter a command\n2.Communicate with a gateway\n3.Communicate with a node");
}

void loop() {
  reader = read_from_console(reader); //read unformatted input
  if (reader.length() >0) { //if theres input in the serial monitor
    reader = format_console_input(reader); //format input
    delay(1000);
    if(reader.equals("1")){ //1: We are entering a command
      print_to_console("Option 1: Enter a Command");
      reader = "";
      while (!(reader.length() >0)){reader = read_from_console(reader);} //wait for input
      reader = format_console_input(reader); //format input
      send_LoRa_Command(""+ reader); //send string with command
      read_data_from_LoRa_Mod();
      reader = "";
      delay(1000);
      print_to_console("Would you like to:\n1.Enter a command\n2.Communicate with a gateway\n3.Communicate with a node");
      }
    else if(reader.equals("2")){ //We are communicating with gateway
      print_to_console("Option 2: Enter a Payload");
      reader = "";
      while (!(reader.length() >0)){reader = read_from_console(reader);} //wait for input
      reader = format_console_input(reader); //format input
      send_LoRa_data(set_port, reader);
      read_data_from_LoRa_Mod();
      reader = "";
      delay(1000);
      print_to_console("Would you like to:\n1.Enter a command\n2.Communicate with a gateway\n3.Communicate with a node");
      }
    else if(reader.equals("3")){ //We are communicating with a node
      print_to_console("Option 3: Would you like to:\n1.Listen\n2.Send");
      reader = "";
      while (!(reader.length() >0)){reader = read_from_console(reader);} //wait for input
      reader = format_console_input(reader); //format input
      if(reader.equals("1")){ //We are listening for another node
        print_to_console("Option 3-1: Listening");
        reader = "";
        send_LoRa_Command("mac pause");
        read_data_from_LoRa_Mod();
        rep: //starting point to repeat the code again
        send_LoRa_Command("radio rx 0");
        read_data_from_LoRa_Mod();
        print_to_console("When finished listening, please enter 0, or to repeat, please enter 1");
        while (!(reader.length()>0) && !(receive.length()>0)){//wait for payload, or reset cue
          receive = listen_for_radio(receive); //listening to lora mod, print to serial monitor
          reader = read_from_console(reader); //listening for input
          } 
          reader = format_console_input(reader); //format input
        if(reader.equals("0")){ //We are reseting
          send_LoRa_Command("mac resume");
          read_data_from_LoRa_Mod();
          reader = "";
          delay(1000);
          print_to_console("Would you like to:\n1.Enter a command\n2.Communicate with a gateway\n3.Communicate with a node");
          }
        else if(reader.equals("1")){
          print_to_console("Repeating the listening command");
          reader = "";
          goto rep;
          }
        else{ //Didn't get cue to reset
          print_to_console("Error: Invalid Entry, Not the reset cue, Repeating");
          reader = "";
          goto rep;
          } 
        }
      else if(reader.equals("2")){ //We are sending to another node
        reader = "";
        send_LoRa_Command("mac pause");
        read_data_from_LoRa_Mod();
        send_LoRa_Command("radio set pwr 14");
        read_data_from_LoRa_Mod();
        sendNode:
        print_to_console("Option 3-2: Would you like to:\n1.Send a continuous wave\n2. Send a single message\n3. Send continuous messages");
        while (!(reader.length() >0)){reader = read_from_console(reader);} //wait for input
        reader = format_console_input(reader); //format input
        if(reader.equals("1")){ //We are sending a continuous wave
          print_to_console("Option 3-2-1: Sending Continuous Wave");
          reader = "";
          send_LoRa_Command("radio cw on");
          read_data_from_LoRa_Mod();
          print_to_console("When finished sending this continuous signal, please enter 0");
          while (!(reader.length()>0)){reader = read_from_console(reader);} //wait for reset cue
          reader = format_console_input(reader); //format input
          if(reader.equals("0")){ //We are reseting
            send_LoRa_Command("radio cw off");
            read_data_from_LoRa_Mod();
            send_LoRa_Command("mac resume");
            read_data_from_LoRa_Mod();
            reader = "";
            delay(1000);
            print_to_console("Would you like to:\n1.Enter a command\n2.Communicate with a gateway\n3.Communicate with a node");
            }
          }
        else if(reader.equals("2")){
          print_to_console("Option 3-2-2: Sending a single message");
          reader = "";
          print_to_console("Enter a Payload");
          while (!(reader.length() >0)){reader = read_from_console(reader);} //wait for input
          reader = format_console_input(reader);
          send_LoRa_Command("radio tx "+ reader);
          read_data_from_LoRa_Mod();
          reader = "";
          delay(1000);
          print_to_console("Would you like to:\n1.Enter a command\n2.Communicate with a gateway\n3.Communicate with a node");
          }
        else if(reader.equals("3")){
          print_to_console("Option 3-2-3: Sending continuous messages");
          reader = "";
          unsigned long counter = 1000000;
          print_to_console("When finished sending, please enter 0");
          while (!(reader.length()>0)){//wait for reset cue
            String txStr = "radio tx ";
            reader = read_from_console(reader); //listening for input
            counter++;
            txStr.concat(counter);
            send_LoRa_Command(txStr);
            read_data_from_LoRa_Mod();
            delay(500);
            } 
          reader = format_console_input(reader); //format input
          if(reader.equals("0")){ //We are reseting
            send_LoRa_Command("mac resume");
            read_data_from_LoRa_Mod();
            reader = "";
            delay(1000);
            print_to_console("Would you like to:\n1.Enter a command\n2.Communicate with a gateway\n3.Communicate with a node");
            }
          }
      else{ //Didnt get listen or send cue
        print_to_console("Error: Invalid Entry, Didn't get listen or send cue");
        reader = "";
        print_to_console("Would you like to:\n1.Enter a command\n2.Communicate with a gateway\n3.Communicate with a node");
        }
      }
    else{ //Didnt get right cue to start one of the three options
      print_to_console("Error: Invalid Entry, Could not pick one of the three options");
      reader = "";
      print_to_console("Would you like to:\n1.Enter a command\n2.Communicate with a gateway\n3.Communicate with a node");
      }
  }
}
}

void InitializeSerials(long baudrate)
{
  Serial.begin(baudrate);
  Serial1.begin(baudrate);
  delay(1000);
  print_to_console("Serial ports initialised");
}

void initializeRN2903A(int pwr_port, int rst_port)
{
  //Enable power to the RN2903A
  pinMode(pwr_port, OUTPUT);
  digitalWrite(pwr_port, HIGH);
  print_to_console("RN2903A Powered up");
  delay(1000);
  
  //Disable reset pin
  pinMode(rst_port, OUTPUT);
  digitalWrite(rst_port, HIGH);

  //Configure LoRa module
  send_LoRa_Command("sys reset");
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac set nwkskey " + set_nwkskey);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac set appskey " + set_appskey);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac set devaddr " + set_devaddr);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac get deveui");
  print_to_console("DevEUI");

  //For this commands some extra delay is needed.
  send_LoRa_Command("mac set adr on");
  read_data_from_LoRa_Mod();
  //send_LoRa_Command("mac set dr 0"); //uncomment this line to fix the RN2903A on SF12 (dr=DataRate)
  //read_data_from_LoRa_Mod();
  delay(1000);

  send_LoRa_Command("mac save");
  delay(1000);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac join abp");
  delay(1000);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("radio set crc off");
  delay(1000);
  read_data_from_LoRa_Mod();
  
  send_LoRa_Command("radio set sf " + spreadFactor);
  delay(1000);
  read_data_from_LoRa_Mod();
  
  send_LoRa_Command("radio set cr " + codeRate);
  delay(1000);
  read_data_from_LoRa_Mod();

}

void print_to_console(String message){
  Serial.println(message);
}

void read_data_from_LoRa_Mod(){
  if (Serial1.available()) {
    String inByte = Serial1.readString();
    Serial.println(inByte);
  }
}

void send_LoRa_Command(String cmd){
  print_to_console("Now sending: " + cmd);
  Serial1.println(cmd);
  delay(500);
}

void send_LoRa_data(int tx_port, String rawdata){
  send_LoRa_Command("mac tx uncnf " + String(tx_port) + String(" ") + rawdata);
}

//String read_from_console(String readVar){
//  while (Serial.available()) {
//    delay(10);  //delay to allow buffer to fill 
//    if (Serial.available()>0) {
//      char c = Serial.read();  //gets one byte from serial buffer
//      readVar += c; //makes the string 
//    } 
//  }
//  return readVar;
//}

String read_from_console(String readVar){
  if (Serial.available()) {
    readVar = Serial.readString();
  }
  return readVar;
}

String format_console_input(String input){
   input.replace("\r","");
   input.replace("\n","");
   //print_to_console("Read: " + input);
   return input;
}

void set_radio_com(){
    // read from lora mod, send to port serial monitor:
  if (Serial1.available()){
    String inByte = Serial1.readString();
    Serial.print(inByte);
    }
  // read from port serial monitor, send to lora mod:
  if (Serial.available()){
    int inByte = Serial.read();
    Serial1.write(inByte);
    }
}

String listen_for_radio(String readVar){
      // read from lora mod, send to port serial monitor:
  if (Serial1.available()){
    String inByte = Serial1.readString();
    Serial.print(inByte);
    }
  return readVar;
}
