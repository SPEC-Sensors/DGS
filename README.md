# DGS
SPEC Sensors' Digital Gas Sensor Library for Arduino (DGS/DSDK/DULP)

Created by David E. Peaslee, March 29, 2018.
Created for SPEC/KWJ Digital Gas Sensor (DGS, or also known as DULPSM)

This is the library for the SPEC Sensors Digital Gas Sensor (DGS), also known as DGSDK, or previously DULP.
This library has no expressed guarantee or warranty and is free to use or distribute.
This version was created March 31st, 2018, with previous version in 2016. 
This library was created by David Peaslee, with much help from the interweb. 

DGS Sensor Library - Example use of library for reading KWJ Engineering digital potentiostat with SPEC Sensors on Digital SDK.


!!! RX, TX Must be on 3.3 volt communication, or using level shifters to get to 3.3V UART!!!

Wiring:
Connect the TX of Digital Sensor to the RX of 3.3 V Arduino
Connect the RX of Digital Sensor to the TX of 3.3 V Arduino
Connect the power of Digital Sensor to the 3.3V power of the Arduino
Connect the ground of Digital Sensor to the ground of the Arduino

This code is optimized for the Arduino Due, but can be modified to work with any other Arduino.
The DGS is a 3.3V device, which means you must use level shifters if using with a 5V Arduino.

SoftwareSerial is needed for any Arduino compatible device that does not have a native alternative serial port (Serial1).  DGS runs at 9600 baud rate.


These examples require the DGS libraries (EXCEPT for DSDK_UART), if it does not compile look to the library.properties file and add your board to the architectures.
The purpose of this example is to help you set up the DGS. After the first setup, loading the barcode into the DGS, you do not need to repeat the setup portion of the DGS commands. You only need to initialize the serial ports in Setup.

In DSDK_SETUP you can re-zero at anytime by sending Z through the serial terminal, once the continuous output starts. If you require technical assistance, please grab a screen shot of the BarCode text, as well as the output of the EEPROM, and the Firmware Date. To turn off verbose setup, change DEBUG to false.

DSDK_Photon is for the particle photon microcontroller, but works with similar code to DSDK_SETUP.

DSDK_RUN2 will work if your board is capable of supporting 2 UART devices at the same time. Arduino Due is an excelent chouce for this example.

DSDK_Photon tested on Phonton.
DSDK_RUN2 tested using Serial1 and Serial2 of Arduino Due.
DSDK_SETUP tested on Due, and 3.3V Arduino Pro Mini (using softwareSerial, instead of Serial1).
DSDK_UART tested on Due, Photon, and 3.3V Arduino Pro Mini (using softwareSerial, instead of Serial1).
