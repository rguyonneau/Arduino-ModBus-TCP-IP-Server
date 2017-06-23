# Arduino-ModBus-TCP-IP-Server

Here is presented a source code to use an Arduino board (an Arduino Uno in my case) as a Modbus TCP/IP Server. The idea is to use the Modbus PDU processing presented here (https://github.com/rguyonneau/Modbus-Server), and adding the Arduino and Ethernet specifications.

Here is a list of the materiel I used:

    Arduino Uno, Model R3

    Base Shield v1.3b by Seeed Studio, to connect the inputs/outputs

    Inputs/outputs

        2 LED sockets v1.2b

        1 Vibration motor v1.2b

        1 Rotary Angle Sensor v1.2b

    Ethernet Shield, Model R3

## Modbus Server files

Thoses files are already presented here (https://github.com/rguyonneau/Modbus-Server). The ModbusServer class allows to process a Modbus PDU request, while the MemInterface class is an interface to manage the memory access. 

## ModBus TCP/IP

Using ModBus TCP/IP means dealing with the ModBus TCP/IP ADU (Application Data Unit) in addition to the ModBus PDU (Protocol Data Unit). More information can be found in the ModBus Messaging On TCP/IP Implementation Guide V1.0b. As we already have a ModBus class that deals with the ModBus PDU, we only need to derivate this class. This derivative class needs to implement a function that deals with the ModBus TCP/IP ADU and then calls the processPDU() function. That is what is done with the ModbusTCPIPServer class presented next.

## Arduino

As I wanted to be able to interract with my inputs/outputs connected to the arduino board, I added some arduino code to the Memory class. The Memory class implements the MemInterface to be compatible with the ModbusServer class. 

## Python client
Here is a python example to interract with the Arduino ModBus TCP/IP server. 
