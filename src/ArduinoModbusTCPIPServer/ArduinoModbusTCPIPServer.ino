
/***
 * This file is part of the Modbus TCP/IP Server for Arduino programm.(V0.1)
 *
 * Copyright (C) 2016 - Remy Guyonneau - remy.guyonneau(at)univ-angers.fr
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/
 
#include <SPI.h>
#include <Ethernet.h>

#include "ModbusTCPIPServer.h"
#include "Memory.h"

// Initialisation of ModBus server parameters: mac adress, ip adress and port number
byte_t mac[] = { 0x34, 0x64, 0xA9, 0xC8, 0xFC, 0xB6 };
IPAddress ip(172, 19, 81, 88);
int port = 502;
EthernetServer my_server(port);// To communicate over an Ethernet connexion

//Declaration the ModBus request (from the client) and the ModBus answer (to the client)
#define MAX_SIZE_TRAME 261
byte_t request_trame[MAX_SIZE_TRAME];
byte_t answer_trame[MAX_SIZE_TRAME];

Memory my_mem = Memory(); //To handle the memory
ModbusTCPIPServer my_modbus(&my_mem); //To process ModBus data

//function to display an array in hexa values
void display_in_hexa(const byte_t* tab, int len){
  for(int i=0; i<len; i++){
    if(tab[i]<0x10) Serial.print('0');
    Serial.print(tab[i],HEX);
    Serial.print(" ");
  }
}

//function to display an array in hexa values and return to the next line after that
void displayln_in_hexa(const byte_t* tab, int len){
  display_in_hexa(tab, len);
  Serial.println();
}

//The Arduino setup() function
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  //Memory.initIO();
  
  //start the tcp server
  Ethernet.begin(mac, ip);
  my_server.begin();
}

//to get an entire trame from the client and return its size
int get_trame(EthernetClient* pclient, byte_t* trame){
  int i = 0;
  char c;
  do{
    c = pclient->read();
    trame[i]=c;
    i++;
  }
  while(pclient->available());
  return i;
}


void loop() {
  // listen for incoming clients
  EthernetClient client = my_server.available();
  if (client) {
    Serial.println("\n-------New client connected\n------");

    while (client.connected()) {
      if (client.available()) {
        //update the inputs (before the client try to read the memory)
        my_mem.updateInputs();
        Serial.println("\n-------New trame\n------");
        //get a trame from the client
        int len_request = get_trame(&client, request_trame);
        Serial.print("request:"); 
        displayln_in_hexa(request_trame, len_request);
        //process the modbus data
        int len_ans = my_modbus.processModbusRequest(request_trame, len_request, answer_trame);
        Serial.print("response:"); 
        displayln_in_hexa(answer_trame, len_ans);
        //answer to the client
        client.write(answer_trame, len_ans);
        //update the output (after the client has updated the memory)
        my_mem.updateOutputs();
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
