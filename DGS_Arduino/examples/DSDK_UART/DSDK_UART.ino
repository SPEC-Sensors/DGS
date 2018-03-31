/*
DSDK_UART - Example use of library for reading KWJ Engineering with SPEC Sensors on Digital SDK.
Created by David E. Peaslee, March 29, 2018.
Created for SPEC/KWJ Digital Gas Sensor (DGS, or also known as DULPSM)
Not yet released into the public domain.

!!! RX, TX Must be on 3.3 volt communication, or using level shifters to get to 3.3V UART!!!

Wiring:
Connect the TX of Digital Sensor to the RX of 3.3 V Arduino
Connect the RX of Digital Sensor to the TX of 3.3 V Arduino
Connect the power of Digital Sensor to the 3.3V power of the Arduino
Connect the ground of Digital Sensor to the ground of the Arduino

This code is optimized for the Arduino Due, but can be modified to work with any other Arduino.
The DGS is a 3.3V device, which means you must use level shifters if using with a 5V Arduino.
SoftwareSerial is needed for any Arduino compatible device that does not have a native alternative serial port (Serial1)
DGS runs at 9600 baud rate.
*/


/* This program is intended to allow serial communication with the DGS. Using the Arduino Serial terminal you can send and recieve information to the DGS:
 * This example requires no extra libraries. 
 * See the documentation page here for a list of commands: 
 * http://www.spec-sensors.com/wp-content/uploads/2017/01/DG-SDK-968-045_9-6-17.pdf
 * When reseting be sure the continuous output has stopped.
 */

//#include "SoftwareSerial.h" //Include the next lines if using Arduino without Serial1 support, and replace refernces to Serial1 with mySerial
//
//SoftwareSerial mySerial(10, 11); // !!! RX, TX Must be on 3.3 volt communication, or using level shifters to get to 3.3V UART!!!

int timing = 300; //set the moving average

void setup() //Setup intended for SDK which is spanned and zeroed, or has the barcode information previously entered. Must initialize both serial ports:
{
  Serial1.begin(9600);
  Serial.begin(9600);
  delay(5000);

  Serial.flush();
  Serial1.flush();
  
  delay(1000);
  Serial1.write('A');
  delay(1000);
  Serial1.print(timing);
  delay(500);
  Serial1.write('\r');
  delay(2000);

  while (Serial1.available()) // read from SDK port, send to Serial port to interupt send 'c' without line ending
  {
    int inByte = Serial1.read();
    Serial.write(inByte);
  }
  Serial.println();


  Serial.println("Finished Setup");
  Serial.println("Sensor #, Conc.(PPB), Temp.(C), Rh (%),Conc. (Counts), Temp. (Counts), Rh (%Counts), Days, Hours, Minutes, Seconds");
  Serial1.write('c');

}

void loop()
{

  while (Serial1.available()) // read from SDK port, send to Serial port to interupt continuous output send 'c' without line ending, may have to send more than once.
  {
    int inByte = Serial1.read();
    Serial.write(inByte);
  }
  while (Serial.available()) // read from Serial port, send to port SDK port
  {
    int inByte = Serial.read();
    Serial1.write(inByte);
    delay(100);
  }
}
